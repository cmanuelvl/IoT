#include <Arduino.h>
#include <BarraLeds.h>
#include <hardware.h>
#include <TimerEvent.h>
#include <LightSensor.h>

#define TMR_500ms 500
#define TMR_400ms 400

const int ledPins[6] = {BG_LED0, BG_LED1, BG_LED2, BG_LED3, BG_LED4, BG_LED5};

BarraLeds barraLeds(6, ledPins);
LightSensor lightSensor(3.3, 10, LIGHT_SENSOR);

TimerEvent tmrid_500ms;
TimerEvent tmrid_400ms;

void setup() {
  // put your setup code here, to run once:
  barraLeds.initialize();
  lightSensor.initialize();
  tmrid_500ms.set(TMR_500ms, tmr_Callback_500ms);
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);

}

void loop() {
  // put your main code here, to run repeatedly:
  tmrid_500ms.update();
  tmrid_400ms.update();
}

void tmr_Callback_500ms(){
  //barraLeds.ledToggle(0);
}

void tmr_Callback_400ms(){
  int scaledValue = lightSensor.getScaledValue();
  barraLeds.barraLedOn(scaledValue); 
}
