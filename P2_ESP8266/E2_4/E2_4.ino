#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>  // permite conectar a la red Wifi
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <TimerEvent.h>
#include <TimeOut.h>
#include <Buzzer.h>
#include "hardware.h"

// define
#define TMR_1s   1000
#define TMR_9s   9000

const char *ssid = "iPhone de cm";
const char *password = "lavacadicemuu";

TimerEvent tmrid_1s;
TimeOut tmrid_9s;

ESP8266WebServer server ( 80 );
Buzzer oBuzzer(GPIO_PIN_5);

int cnt = 0;
int cntb;
String webPage;

void tmr_Callback_1s ();
void tmr_Callback_9s ();


//Callback para actualizar el valor de la exposición
void handleExposicion() {
  String exposicionValue = String(cntb);
  
  Serial.print ( "klk") ;
  Serial.println ( "exposicionValue") ;
  
  server.send(200, "text/plane", exposicionValue);

  //if (ok leer valor)
  //else 
  // error
}

// Calback recibir solicitudes, respuesta ack 200ok + webPage 
void handleRoot() {
  if (server.method() == HTTP_POST) {
    String postData = server.arg("umbral");
    int newThreshold = postData.toInt(); // Convertir a entero

    // Almacenar el nuevo umbral (ejemplo)
    umbralActual = newThreshold;

    // Enviar una respuesta al cliente
    server.send(200, "text/plain", "Umbral actualizado correctamente");
  } else {
    server.send(200, "text/html", webPage);
  }
  server.send(200, "text/html", webPage);
}
  
void setup() {

  Serial.begin ( 115200 );
  
  webPage = "<html><head><title>Exposicion Luminica</title><style>body {text-align: center;}</style></head>";
  webPage += "<body><header><h1>Exposicion Luminica</h1></header><main><p>Exposicion: <span id=\"exposicionValue\"></span>%</p>";
  webPage += "<p>Umbral establecido: <span id=\"umbralValue\"></span></p> <form action=\"/establecer_umbral\" method=\"POST\">";
  webPage += "<label for=\"umbral\">Umbral:</label><input type=\"number\" id=\"umbral\" name=\"umbral\" required min=\"0\" max=\"100\">";
  webPage += "<button type=\"submit\">Enviar</button></form></main><script>function getExposicionData(){const xhttp = new XMLHttpRequest();";
  webPage += "xhttp.open(\"GET\", \"readExposicion\", true);xhttp.onload = function() {if (this.status === 200)";
  webPage += "{document.getElementById(\"exposicionValue\").innerHTML = this.responseText;} else {";
  webPage += "console.error(\"Error al obtener los datos:\", this.statusText);}};";
  webPage += "xhttp.onerror = function() {console.error(\"Error de red:\", this.statusText);};";
  webPage += "xhttp.send();}setInterval(getExposicionData, 1000);</script></body></html>";
  
   pinMode(GPIO_PIN_2, OUTPUT);
   digitalWrite(GPIO_PIN_2, HIGH);
   
  // Inicializacion WIFI
  WiFi.begin (ssid, password);
  Serial.println ( "" );
  while ( WiFi.status () != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " ) ;
  Serial.println ( ssid );
  Serial.print ( "IP address: " ) ;
  Serial.println ( WiFi.localIP() ) ;

  // Aviso de conexion
  digitalWrite(GPIO_PIN_2, LOW);
  tmrid_1s.set(TMR_1s, tmr_Callback_1s);
  tmrid_9s.timeOut(TMR_9s, tmr_Callback_9s); 
  oBuzzer.buzzerOn(350);
    
  server.on("/", handleRoot);   // escuchando solicitud de pagina 
  server.on("/readExposicion", handleExposicion); // escuchando solicitud de nivel de exposicion 
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly
  tmrid_1s.update();
  TimeOut::handler();
  
  server.handleClient();
  
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
   cntb =rand()%101;
   Serial.print ( "este" );
   Serial.println ( cntb );
}

void tmr_Callback_9s (){
  //tmrid_1s.disable();
  tmrid_9s.cancel();
  Serial.print ( "9s") ;
  oBuzzer.buzzerOff();
}
