#include <TFT_eSPI.h> // Hardware-specific library

#include "Free_Fonts.h"

#define BUTTON D0

TFT_eSPI tft = TFT_eSPI(); 
    
void setup() {
  pinMode(D0, INPUT);
  tft.init();
  tft.setFreeFont(FSSBO24);
}

void drawScene() {
  char dims[32];
  snprintf(dims, 32, "screen (%d,%d)", tft.width(), tft.height());
  // put your main code here, to run repeatedly:
  tft.setRotation(3);
  tft.drawString(dims, 25, 25);
}

void loop() {
  int buttonState = analogRead(BUTTON);
  float val = buttonState/1023.0;
  if (val <= .25) {
    tft.setFreeFont(FSSBO9);
  } else if (val <= .50) {
    tft.setFreeFont(FSSBO12);
  } else if (val <= .75) {
    tft.setFreeFont(FSSBO18);
  } else {
    tft.setFreeFont(FSSBO24);
  }

  drawScene();
  delay(1000);
}
