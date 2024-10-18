#include <ESP8266WiFi.h>  // permite conectar a la red Wifi
#include "hardware.h"
#include <Buzzer.h>
#include <TimerEvent.h>
#include <TimeOut.h>

// define
#define TMR_1s   1000
#define TMR_9s   9000

const char *ssid = "iPhone de cm";
const char *password = "lavacadicemuu";

TimerEvent tmrid_1s;
TimeOut tmrid_9s;

WiFiServer server ( 80 );
Buzzer oBuzzer(GPIO_PIN_5);

int cnt = 0;

void tmr_Callback_1s ();
void tmr_Callback_9s ();

void setup() {

   pinMode(GPIO_PIN_2, OUTPUT);
   digitalWrite(GPIO_PIN_2, HIGH);
   
  // Inicializacion WIFI
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status () != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  
  tmrid_1s.set(TMR_1s, tmr_Callback_1s);
  tmrid_9s.timeOut(TMR_9s, tmr_Callback_9s); 
  oBuzzer.buzzerOn(350);
  
  Serial.println ( "" );
  Serial.print ( "Connected to " ) ;
  Serial.println ( ssid );
  Serial.print ( "IP address: " ) ;
  Serial.println ( WiFi.localIP() ) ;
  digitalWrite(GPIO_PIN_2, LOW);
}

void loop() {
  // put your main code here, to run repeatedly
  tmrid_1s.update();
  TimeOut::handler();
  
  if ( WL_CONNECTED != WiFi.status() ){
    Serial.print ( WiFi.status() ) ;
    digitalWrite(GPIO_PIN_2, HIGH);
  }
}

void tmr_Callback_1s (){
  cnt = (cnt >= 2) ? 0 : cnt+1;
  if (0 == cnt)
    oBuzzer.buzzerOn(350);
  else 
    oBuzzer.buzzerOff();
}

void tmr_Callback_9s (){
  tmrid_1s.disable();
  tmrid_9s.cancel();
  Serial.print ( "9s") ;
  oBuzzer.buzzerOff();
}
