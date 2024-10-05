#include <Arduino.h>
#include <BarraLeds.h>
#include <hardware.h>
#include <TimerEvent.h>
#include <TimeOut.h>
#include <LightSensor.h>
#include <Buzzer.h>
#include <Button.h>
#include <Rgb.h>

// defines
#define TMR_500ms 500
#define TMR_400ms 400
#define TMR_10s   10000
#define TMR_20s   20000
#define TMR_30s   30000

#define UMBRAL 75 

// constantes 
const int ledPins[6] = {PIN_BG_LED0, PIN_BG_LED1, PIN_BG_LED2, PIN_BG_LED3, PIN_BG_LED4, PIN_BG_LED5};
const int rgbPins[3] = {PIN_RGB_R, PIN_RGB_G, PIN_RGB_B};

// tipos
typedef enum {NORMAL, ALARMA_RGB, ALARMA_BUZZER, WAIT} t_estados;
t_estados estado;

// objetos
BarraLeds   oBarra(ledPins,6);
LightSensor oLight(PIN_LIGHT_SENSOR, 3.3, 10);
Buzzer      oBuzzer(PIN_BUZZER);
Button      oButton(PIN_BUTTON);
Rgb         oRgb(rgbPins);

TimerEvent tmrid_500ms;
TimerEvent tmrid_400ms;
TimeOut tmrid_30s;
TimeOut tmrid_20s;
TimeOut tmrid_10s;

// variables
float percent;  // porcentaje de exposición
int numLeds;  // numero de led encendidos de la barra de leds
bool toggle; // toggle del rgb
bool buzzerOn;
void tmr_Callback_500ms();
void tmr_Callback_400ms();
void tmr_Callback_10s();
void tmr_Callback_20s();
void tmr_Callback_30s();

void setup() {
  // put your setup code here, to run once:
  oBarra.initialize();
  oLight.initialize();
  oBuzzer.initialize();
  oButton.initialize();
  estado = NORMAL;
  toggle = true;
  buzzerOn = false;
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);
}

void loop() {
  // put your main code here, to run repeatedly:
  tmrid_400ms.update();
  tmrid_500ms.update();
  TimeOut::handler();
  
  switch(estado){
    case NORMAL: // Estado NORMAL, se realiza la medida de la luminancia cada 400 ms y se refleja en la barra 
      if(percent >= UMBRAL){
        estado = ALARMA_RGB;
        tmrid_10s.timeOut(TMR_10s, tmr_Callback_10s); 
        tmrid_20s.timeOut(TMR_20s, tmr_Callback_20s);
      }
    break; 

    case ALARMA_RGB: // Estado ALARMA_RGB, inicio de tiempo de exposicion, a la mitad del tiempo (20s/2) se enciende RGB
      if (percent < UMBRAL ){
        estado = NORMAL;
        
        tmrid_10s.cancel();
        tmrid_20s.cancel();

        tmrid_500ms.disable();
        oRgb.rgbColor(0, 0, 0);
     }
    break;
    
    case ALARMA_BUZZER: // Estado ALARMA_BUZZER, cuando se lleven 20s pasamos a encender el altavoz, solo se puede apagar mediante el pulsador
      if(oButton.buttonRead() == LOW){
          estado = WAIT;
          
          tmrid_30s.timeOut(TMR_30s, tmr_Callback_30s);
          
          tmrid_500ms.disable();
          oRgb.rgbColor(0, 0, 0);
          tmrid_400ms.disable();
          oBarra.barraLedOn(0);
          oBuzzer.buzzerOff();
          buzzerOn = false;
       }
    break;

    case WAIT: // Estado WAIT, se espera 30s para un inicir nuevamente el sistema
      
    break;
  }
}

void tmr_Callback_400ms(){
  percent = oLight.getPercent();
  numLeds = round(percent*6/100);
  oBarra.barraLedOn(numLeds);
}

void tmr_Callback_500ms(){
  if(toggle){
    oRgb.rgbColor(0, 255, 0);
    if (buzzerOn)
      oBuzzer.buzzerToggle(350, 500);
  } else {
    oRgb.rgbColor(0, 0, 0);
    if (buzzerOn)
      oBuzzer.buzzerToggle(350, 500);
  }
    
  toggle =! toggle;
}

void tmr_Callback_10s(){
  oRgb.rgbColor(0, 255, 0);
  tmrid_500ms.set(TMR_500ms, tmr_Callback_500ms);
  toggle = false;
}

void tmr_Callback_20s(){
  buzzerOn = true;
  oBuzzer.buzzerToggle(350, 500);
  estado = ALARMA_BUZZER;
}

void tmr_Callback_30s(){
  estado = NORMAL;
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);
}
