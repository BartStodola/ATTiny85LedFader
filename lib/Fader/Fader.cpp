#include "Fader.h"
#include <Math.h>

Fader::Fader()
{
  InitBrightness(brightnessArray, &brightnessArrayMax);
}

void Fader::FadeIn(int msec)
{}

void Fader::FadeOut(int msec)
{}

void Fader::InitBrightness(float *arr, int *max)
{
  const int pwmIntervals = 100;
  // Calculate the R variable (only needs to be done once at setup)
  float R = (pwmIntervals * log10(2))/(log10(255));

  // ramp up
  int i=0;
  for (int interval = 0; interval <= pwmIntervals; interval++)
  {
      // Calculate the required PWM value for this interval step
      int brightness = Math.pow (2, (interval / R)) - 1;
      if (brightness < 2) continue;
      // Set the LED output to the calculated brightness
      arr[i++] = brightness;
  }
  for (int j = i, k=0; j<2*i;j++,k++) arr[j] =  arr[i-k];

  *max = i*2;
}
