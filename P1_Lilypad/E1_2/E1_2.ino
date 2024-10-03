#include <Arduino.h>
#include <BarraLeds.h>
#include <hardware.h>
#include <TimerEvent.h>
#include <LightSensor.h>
#include <Buzzer.h>
#include <Button.h>
#include <TimeOut.h>
#include <Rgb.h>

// defines
#define TMR_500ms 500
#define TMR_400ms 400
<<<<<<< HEAD
=======
#define TMR_10000ms 10000
#define TMR_20000ms 20000
#define UMBRAL 50
>>>>>>> 3a18c365e66f216cab33cb2a1fff96cae959e6e8

#define TMR_10s   10000
#define TMR_20s   20000
#define TMR_30s   30000

#define UMBRAL 50   // en %

// constantes 
const int ledPins[6] = {BG_LED0, BG_LED1, BG_LED2, BG_LED3, BG_LED4, BG_LED5};
const int rgbPins[3] = {RGB_R, RGB_G, RGB_B};

// tipos
typedef enum {NORMAL, ALARMA, WAIT} t_estados;
t_estados estado;

// objetos
BarraLeds   oBarra(ledPins,6);
LightSensor oLight(LIGHT_SENSOR, 3.3, 10);
Buzzer      oBuzzer(BUZZER);
Button      oButton(BUTTON);
Rgb         oRgb(rgbPins);

TimerEvent tmrid_500ms;
TimerEvent tmrid_400ms;
<<<<<<< HEAD
TimeOut tmrid_30s;
TimeOut tmrid_20s;
TimeOut tmrid_10s;

int nivel;  // Nivel de exposición

void tmr_Callback_500ms();
void tmr_Callback_400ms();
void tmr_Callback_10s();
void tmr_Callback_20s();
void tmr_Callback_30s();
=======
TimerEvent tmrid_10000ms;
TimerEvent tmrid_20000ms;
>>>>>>> 3a18c365e66f216cab33cb2a1fff96cae959e6e8

void setup() {
  // put your setup code here, to run once:
  oBarra.initialize();
  oLight.initialize();
  oBuzzer.initialize();
  oButton.initialize();
  
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);
}

void loop() {
  // put your main code here, to run repeatedly:
  tmrid_400ms.update();
  tmrid_500ms.update();
  
  TimeOut::handler();
  
  switch(estado){
<<<<<<< HEAD
    case NORMAL: // Estado inicial, se realiza la medida de la luminancia cada 400 ms y se refleja en la barra 

      if(nivel >= UMBRAL){
        estado = ALARMA;
        tmrid_10s.timeOut(TMR_10s, tmr_Callback_10s); // se empieza a contar el tiempo de exposicion, a la mitad del tiempo (20s/2) se enciende RGB Cuando se lleven 10s encender rgb, y 
        tmrid_20s.timeOut(TMR_20s, tmr_Callback_20s); // cuando se lleven 20s pasamos al estado ALARMA enciendo el altavoz 
      }
    break; 

    case ALARMA: 
      if (nivel < UMBRAL || oButton.buttonRead() == LOW){
        estado = WAIT;
        
        tmrid_30s.timeOut(TMR_30s, tmr_Callback_30s); // espera 30s antes de volver al estado NORMAL

        // desactivamos todo y pasamos a WAIT
        tmrid_500ms.disable();
        //digitalWrite(RGB_G, LOW);
        oRgb.rgbColor(0, 0, 0);
        tmrid_400ms.disable();
        oBarra.barraLedOn(0);
        
        oBuzzer.buzzerOff();
        
       }
    break;

    case WAIT: // Estado ALARMA encendida, se activa el altavoz porque se ha superado 20s por encima del UMBRAL, solo se puede apagar mediante el pulsador
      
=======
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
>>>>>>> 3a18c365e66f216cab33cb2a1fff96cae959e6e8
    break;
  }
}

<<<<<<< HEAD
void tmr_Callback_400ms(){
  nivel = oLight.getScaledValue();
  oBarra.barraLedOn(nivel);
=======
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
>>>>>>> 3a18c365e66f216cab33cb2a1fff96cae959e6e8
}

void tmr_Callback_500ms(){
//  oRgb.rgbToggle();
}

void tmr_Callback_10s(){
  oRgb.rgbColor(0, 255, 0); //digitalWrite(RGB_G, HIGH);
  tmrid_500ms.set(TMR_500ms, tmr_Callback_500ms);
}

void tmr_Callback_20s(){
  oBuzzer.buzzerToggle(350,500);
}

void tmr_Callback_30s(){
  estado = NORMAL;
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms); 
}
