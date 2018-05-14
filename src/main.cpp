#include <Arduino.h>
#include "Fader.h"
// for ATTiny85
// on button pressed fade led in stay for n-sec. and fade out
Fader fader = Fader();

const int triggerPin =0;//LED
const int ledPin = 1;  // LED
const int inputPin = 2;// button or PIR-Sensor
const int potiPin = 3;

const int maxarr= 64;
int brightnessArrayMax = 0;
float brightnessArray[maxarr];

int processing = -1;
int maxProcessingCount = 10;

void InitBrightness(float *arr, int *max)
{
  const int pwmIntervals = 100;
  // Calculate the R variable (only needs to be done once at setup)
  float R = (pwmIntervals * log10(2))/(log10(255));

  // ramp up
  int i=0;
  for (int interval = 0; interval <= pwmIntervals; interval++)
  {
      // Calculate the required PWM value for this interval step
      int brightness = pow (2, (interval / R)) - 1;
      if (brightness < 2) continue;
      // Set the LED output to the calculated brightness
      arr[i++] = brightness;
  }
  for (int j = i, k=0; j<2*i;j++,k++) arr[j] =  arr[i-k];

  *max = i*2;
}
void FadeIn(int delayMsec)
{
  for (size_t i = 0; i < brightnessArrayMax/2; i++) {
       analogWrite(ledPin,  brightnessArray[i]);
       delay(delayMsec);
     }
}
void FadeOut(int delayMsec)
{
  for (size_t i = brightnessArrayMax/2+1; i < brightnessArrayMax; i++) {
       analogWrite(ledPin,  brightnessArray[i]);
       delay(delayMsec);
     }
   analogWrite(ledPin, 0);
}

boolean IsButtonPressed()
{
  if (digitalRead(inputPin) == HIGH) {
    // turn LED on:
    return true;
  } else {
    // turn LED off:
    return false;
  }
}

void setup() {
  //Serial.begin(9600);
  InitBrightness(brightnessArray, &brightnessArrayMax);

  pinMode(inputPin, INPUT);
  pinMode(potiPin, INPUT);
  processing = -1;
  //Serial.println("setup");
}

void loop() {
//  Serial.print(IsButtonPressed());
//  Serial.print("\t");
//  Serial.println(maxProcessingCount);
  maxProcessingCount = map(analogRead(potiPin), 0,1023, 0,50);

  if (IsButtonPressed())
  {
    if (processing == -1)
    {
      // fade in
      digitalWrite(triggerPin, HIGH);
      delay(500);
      digitalWrite(triggerPin, LOW);
      //Serial.println("fade in");
      processing = maxProcessingCount;
      FadeIn(20);
    }
    if (processing > -1)
    {
      // set re-trigger count
      //Serial.println("set re-trigger count");
      digitalWrite(triggerPin, HIGH);
      delay(500);
      digitalWrite(triggerPin, LOW);
      processing = maxProcessingCount;
    }
  }

  if (processing > 0)
  {
    // illuminate
    //Serial.println("illuminate");
    digitalWrite(ledPin, HIGH);
    delay(500);
    processing -= 1;
  }
  if (processing == 0)
  {
    // fade out
    //Serial.println("fade out");
    FadeOut(40);
    processing =-1;
  }
}
