#include <Arduino.h>
#include <BarraLeds.h>
#include <hardware.h>
#include <TimerEvent.h>
#include <LightSensor.h>
#include <Buzzer.h>
#include <Button.h>

#define TMR_500ms 500
#define TMR_400ms 400
#define TMR_10000ms 10000
#define TMR_20000ms 20000
#define UMBRAL 50

const int ledPins[6] = {BG_LED0, BG_LED1, BG_LED2, BG_LED3, BG_LED4, BG_LED5};
const int estado[3] = {INICIAL, UMBRAL, ALARMA};

BarraLeds barraLeds(ledPins,6);
LightSensor lightSensor(LIGHT_SENSOR, 3.3, 10);
int barra_value;
Buzzer buzzer(BUZZER);
Button button(BUTTON);

TimerEvent tmrid_500ms;
TimerEvent tmrid_400ms;
TimerEvent tmrid_10000ms;
TimerEvent tmrid_20000ms;

void setup() {
  // put your setup code here, to run once:
  barraLeds.initialize();
  lightSensor.initialize();
  buzzer.initialize();
  button.initialize();
  
  tmrid_500ms.set(TMR_500ms, tmr_Callback_500ms);
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);

}

void loop() {
  // put your main code here, to run repeatedly:
  switch(estado){
    case INICIAL: // Estado inicial, se realiza la medida de la luminancia cada 400 ms y se refleja en la barra 
      light_measure();
      
      if(barra_value >= UMBRAL){
        estado = UMBRAL;
        tmrid_10000ms.set(TMR_10000ms, tmr_Callback_10000ms);// Lanzar un temporizador que cuente tiempo de 10s
        tmrid_20000ms.set(TMR_20000ms, tmr_Callback_20000ms);// Lanzar un temporizador que cuente tiempo de 20s
      }
    break; 

    case UMBRAL: // Estado UMBRAL superado, se empieza a contar el tiempo de exposicion, a la mitad del tiempo (20s/2) se enciende RGB
       light_measure();
       // Cuando se lleven 10s encender rgb, y cuando se lleven 20s pasamos al estado ALARMA enciendo el altavoz 
       
       
       if (barra_value < UMBRAL){
        estado = INICIAL;
        // Borrar la temporizacion
       }
    break;

    case ALARMA: // Estado ALARMA encendida, se activa el altavoz porque se ha superado 20s por encima del UMBRAL, solo se puede apagar mediante el pulsador
      if (button.buttonRead() == 0){
        estado = INICIO;
      }
    break;
  }
  
//  tmrid_500ms.update();
//  tmrid_400ms.update();
//  if (button.buttonRead() == 0){
//    buzzer.buzzerOn(350);
//    delay(1000);
//    buzzer.buzzerOff();
//  }
//}

void tmr_Callback_500ms(){
}

//void tmr_Callback_400ms(){
//  barra_value = lightSensor.getScaledValue();
//  barraLeds.barraLedOn(barra_value);
//}

void tmr_Callback_10000ms(){
  // Encender RGB
}

void tmr_Callback_20000ms(){
  // Encender altavoz
  buzzer.buzzerOn(350);
  delay(1000);
  buzzer.buzzerOff();
}

void light_measure(){
  delay(400);
  barra_value = lightSensor.getScaledValue();
  barraLeds.barraLedOn(barra_value);
}
