#include <Arduino.h>

// for ATTiny85
// on button pressed fade led in stay for n-sec. and fade out

//int brightness[] = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 4, 4, 5, 6, 7, 8, 9, 10, 11, 13, 14, 16, 18, 21, 23, 26, 30, 33, 37, 42, 47, 53, 59, 66, 74, 83, 93, 104, 116, 130, 145, 162, 181, 203, 227, 255 };
int brightness[] = { 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 9, 11, 13, 15, 18, 22, 26, 31, 37, 44, 53, 62, 74, 87, 104, 123, 145, 172, 203, 240, 255 };
//int brightness[] = { 0, 0, 0, 0, 1, 2, 2, 3, 4, 6, 8, 10, 13, 16, 21, 26, 33, 42, 53, 66, 83, 104, 130, 162, 203, 255 };
int count = (sizeof(brightness)/sizeof(*brightness));

const int triggerPin =0;//LED
const int ledPin = 1;  // LED
const int inputPin = 2;// button or PIR-Sensor
const int potiPin = 3;

int processing = -1;
int maxProcessingCount = 10;

void FadeIn(int delayMsec)
{
  for (size_t i = 0; i < brightness; i++) {
       analogWrite(ledPin,  brightness[i]);
       delay(delayMsec);
     }
}
void FadeOut(int delayMsec)
{
  for (size_t i = 0; i < brightness; i++) {
       analogWrite(ledPin,  brightness[count-1-i]);
       delay(delayMsec);
     }
   analogWrite(ledPin, 0);
}

boolean IsTriggerSet()
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
  pinMode(inputPin, INPUT);
  pinMode(potiPin, INPUT);
  processing = -1;
}

void loop() {

  maxProcessingCount = map(analogRead(potiPin), 0,1023, 0,50);

  if (IsTriggerSet())
  {
    if (processing == -1)
    {
      // fade in
      digitalWrite(triggerPin, HIGH);
      delay(500);
      digitalWrite(triggerPin, LOW);

      processing = maxProcessingCount;
      FadeIn(20);
    }
    if (processing > -1)
    {
      // set re-trigger count
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
