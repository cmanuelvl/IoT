#include <Arduino.h>
#include <BarraLeds.h>
#include <hardware.h>

const int ledPins[] = {BG_LED0, BG_LED1, BG_LED2, BG_LED3, BG_LED4, BG_LED5};
BarraLeds barraLeds(6, ledPins);

void setup() {
  // put your setup code here, to run once:
barraLeds.initialize();
}

void loop() {
  // put your main code here, to run repeatedly:
  barraLeds.ledOn(0);
  barraLeds.ledOn(1);
  delay(1000);
  barraLeds.ledOff(0);
  barraLeds.ledOff(1);
 
  delay(1000);
}
