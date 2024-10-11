#include <LowPower.h>
#include <Arduino.h>
#include <BarraLeds.h>
#include <hardware.h>
#include <TimerEvent.h>
#include <TimeOut.h>
#include <LightSensor.h>
#include <Buzzer.h>
#include <Button.h>
#include <Rgb.h>
#include <ADXL335.h>

// defines
#define TMR_500ms 500
#define TMR_400ms 400
#define TMR_10s   10000
#define TMR_20s   20000
#define TMR_30s   30000
#define UMBRAL 90

// constantes 
const int ledPins[6] = {PIN_BG_LED0, PIN_BG_LED1, PIN_BG_LED2, PIN_BG_LED3, PIN_BG_LED4, PIN_BG_LED5};
const int rgbPins[3] = {PIN_RGB_R, PIN_RGB_G, PIN_RGB_B};
const int accPins[3] = {PIN_ADXL335_X, PIN_ADXL335_Y, PIN_ADXL335_Z};

// tipos
typedef enum {INIT, LOW_PWR, NORMAL, ALARMA_RGB, ALARMA_BUZZER, WAIT} t_estados;
t_estados estado;

// objetos
BarraLeds   oBarra(ledPins, NUM_BG_LEDS);
LightSensor oLight(PIN_LIGHT_SENSOR, ADC_VREF, ADC_BITS);
Buzzer      oBuzzer(PIN_BUZZER);
Button      oButton(PIN_BUTTON);
Rgb         oRgb(rgbPins);
ADXL335     oAccel(accPins, ADC_VREF, ADC_BITS);

TimerEvent tmrid_500ms;
TimerEvent tmrid_400ms;
TimeOut tmrid_30s;
TimeOut tmrid_20s;
TimeOut tmrid_10s;

// variables
float percent;          // Porcentaje de exposición
int numLeds;            // Número total de leds encendidos
bool toggle;            // Control del rgb para hacer toggle
bool buzzerOn;          // Control del buzzer para generar freq diferentes  
float ox, oy, oz;       // Offset de las aceleraciones cuando esta en la posicion inicial (reposo)
float x_roll, y_pitch;  // Angulos (grados) del cabeceo y balanceo

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
  oRgb.initialize();
  oAccel.initialize();
  
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);
  
  estado = INIT;
}

void loop() {
  // put your main code here, to run repeatedly:
  tmrid_400ms.update();
  tmrid_500ms.update();
  
  TimeOut::handler();
  
  switch(estado){
    case INIT:
      estado = LOW_PWR;
      Serial.println("LOW_PWR");
      
      // Inicialización de variables
      ox = 0;
      oy = 0;
      oz = 0;
      x_roll = 0; 
      y_pitch = 0; 
      toggle = true;
      buzzerOn = false;
      
      // Cálculo del ofset del accelerometro
      oAccel.getOffset(&ox, &oy, &oz);
      delay(100);
      
    break;

    case LOW_PWR: // Estado LOW_PWR, control del bajo consumo dependiendo del cabeceo y balanceo.

      Serial.println("WAIT");
      
      
      // modo bajo consumo 
      Serial.println("DORMIR ");
      //delay(1000);  //LowPower.powerDown(SLEEP_15MS, ADC_OFF, BOD_OFF); 
      LowPower.idle(SLEEP_1S, ADC_OFF, TIMER4_OFF, TIMER3_OFF, TIMER1_OFF, TIMER0_OFF, SPI_OFF, USART1_OFF, TWI_OFF, USB_OFF);
      Serial.println("DESPERTAR ");
      
      oAccel.getAngle(&ox, &oy, &oz, &x_roll, &y_pitch);
      Serial.print("Ang: ("); Serial.print(x_roll); Serial.print(","); Serial.print(y_pitch); Serial.println(")");

      // consulta pulsera (roll, pitch)
      if(x_roll == 0 && y_pitch == 0){
        estado = NORMAL;
        Serial.println("NORMAL");
        tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);
      }
    break;
    
    case NORMAL: // Estado NORMAL, control de la luminancia y encendidi de la barra cada 400 ms.
      if(percent >= UMBRAL){
        estado = ALARMA_RGB;
        Serial.println("ALARMA_RGB");
        
        tmrid_10s.timeOut(TMR_10s, tmr_Callback_10s); 
        tmrid_20s.timeOut(TMR_20s, tmr_Callback_20s);
      }
    break; 

    case ALARMA_RGB: // Estado ALARMA_RGB, control de los 10 primeros segundos del tiempo de exposicion. Encendido del RGB.
      if (percent < UMBRAL ){
        estado = NORMAL;
        Serial.println("NORMAL");
        
        tmrid_10s.cancel();
        tmrid_20s.cancel();
        tmrid_500ms.disable();
        oRgb.rgbColor(0, 0, 0);
     }
    break;
    
    case ALARMA_BUZZER: // Estado ALARMA_BUZZER, controld de los 10 siguientes segundos. Encendido del buzzer.
      if(oButton.buttonRead() == LOW){
          estado = WAIT;
          Serial.println("WAIT");
          tmrid_30s.timeOut(TMR_30s, tmr_Callback_30s);
          tmrid_500ms.disable();
          oRgb.rgbColor(0, 0, 0);
          tmrid_400ms.disable();
          oBarra.barraLedOn(0);
          oBuzzer.buzzerOff();
          buzzerOn = false;
       }
    break;

    case WAIT: // Estado WAIT, control de los 30s de espera para volver a tomar medidas.
      
    break;
  }
  
  if(x_roll != 0 || y_pitch != 0){
     estado = LOW_PWR;
     Serial.println("LOW_PWR");
     
     // desactivar sensores
     oRgb.rgbColor(0, 0, 0);
     oBarra.barraLedOn(0);
     oBuzzer.buzzerOff();
     buzzerOn = false;

     tmrid_400ms.disable();
     tmrid_500ms.disable();
     tmrid_10s.cancel();
     tmrid_20s.cancel();
  }
}

void tmr_Callback_400ms(){
  percent = oLight.getPercent();
  numLeds = round(percent*6/100);
  oBarra.barraLedOn(numLeds);
  oAccel.getAngle(&ox, &oy, &oz, &x_roll, &y_pitch);  // Control para volver a estado LOW_PWR, si el brazo esta en otra posición
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
  estado = ALARMA_BUZZER;
  Serial.println("ALARMA_BUZZER");
  buzzerOn = true;
  oBuzzer.buzzerToggle(350, 500);
  
}

void tmr_Callback_30s(){
  estado = NORMAL;
  Serial.println("NORMAL");
  tmrid_400ms.set(TMR_400ms, tmr_Callback_400ms);
}
