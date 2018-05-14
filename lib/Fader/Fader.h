#ifndef FADER_H
#define FADER_H

class Fader;

class Fader {
private:
  int brightnessArrayMax = 0;
  float brightnessArray[64];
public:
  Fader();
  void FadeIn(int msec);
  void FadeOut(int msec);
private:
  void InitBrightness(float *arr, int *max);
};

#endif
