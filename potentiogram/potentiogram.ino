#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>
#include"Histogram.h"

#define BUTTON D0

float last = 0.0;
TFT_Histogram histogram=TFT_Histogram();
TFT_eSPI tft = TFT_eSPI(); 
    
void setup() {
  pinMode(D0, INPUT);

  tft.init();                  
  histogram.initHistogram(&tft);                                             //Initialize histogram 
  histogram.formHistogram("Base",1,1.0,40,TFT_RED);                          //Input lable,NO,value(high),width,colour
  histogram.formHistogram("A",2,1.0,40,TFT_GREEN);                          //Input lable,NO,value(high),width,colour
  histogram.showHistogram();

}

void serialOut(float val) {
  Serial.print("val State: ");
  Serial.println(val);
}

void loop() {
  int buttonState = analogRead(BUTTON);
  float val = buttonState/1023.0;
  //serialOut(val);
  if (val != last) {
    histogram.changeParam(2,"A",val,TFT_GREEN);
    histogram.showHistogram();
  }
  delay(100);

}
