#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>  // permite conectar a la red Wifi
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <TimerEvent.h>
#include <TimeOut.h>
#include "hardware.h"
#include <LightSensor.h>

//define
//#define VREF 3.3
#define ADC_BITS  10
#define TMR_500ms 500

//const
const char *ssid = "iPhone de cm";
const char *password = "lavacadicemuu";

//Objetos
ESP8266WebServer server ( 80 );
LightSensor oLight(GPIO_PIN_ADC, 3.3, ADC_BITS);
TimerEvent tmrid_500ms;

int umbralActual, exposicionActual;
String webPage;
  
void setup() {
  Serial.begin ( 115200 );
  
  webPage = "<html><head><title>Exposicion Luminica</title><style>body {text-align: center;}</style></head>";
  webPage += "<body><h1>EXPOSICION LUMINICA</h1><p>Exposicion: <span id=\"exposicionValue\"></span>%</p>";
  webPage += "<form><label for=\"umbral\">Umbral:</label>";
  webPage += "<input type=\"number\" id=\"umbral\" name=\"umbral\" required min=\"0\" max=\"100\"><button type=\"submit\">Enviar</button></form>";
  webPage += "<div id=\"alarma\"><p><strong>Luminosidad alta!</strong></p><button id=\"activarAlarma\">Activar Alarma</button></div>";
  webPage += "<script>function getExposicionData() {fetch('/readExposicion').then(response => response.text()).then(data =>{";
  webPage += "document.getElementById('exposicionValue').textContent = data;}).catch(error => {console.error('Error al obtener los datos:', error);});}";
  webPage += "document.querySelector('form').addEventListener('submit', (event) => {event.preventDefault();";
  webPage += "const umbral = document.getElementById('umbral').value;fetch('/setUmbral', {";
  webPage += "method: 'POST',headers: {'Content-Type': 'application/x-www-form-urlencoded'},body: `umbral=${umbral}`})";
  webPage += ".then(response => {if (response.ok) {console.log('Umbral establecido correctamente');";
  webPage += "document.getElementById('umbralValue').textContent = umbral;} else {console.error('Error al establecer el umbral');}})";
  webPage += ".catch(error => {console.error('Error al enviar el formulario:', error);});});";
  webPage += "getExposicionData();setInterval(getExposicionData, 1000); </script></body></html>";
  
  // Init
  oLight.initialize();
  pinMode(GPIO_PIN_2, OUTPUT);
  umbralActual = 100;
  
  // Iniciando conexion WIFI
  WiFi.begin (ssid, password);
  
  while ( WiFi.status () != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
    digitalWrite(GPIO_PIN_2, !digitalRead(GPIO_PIN_2)); // Aviso LED
  }
  
  // Conexion WIFI realizada
  Serial.println ( "" );
  Serial.print ( "Connected to " ) ;
  Serial.println ( ssid );
  Serial.print ( "IP address: " ) ;
  Serial.println ( WiFi.localIP() ) ;

  // Servidor lanzado
  server.begin();
  server.on("/", handleRoot);   // escuchando solicitud de pagina 
  server.on("/readExposicion", handleExposicion); // escuchando solicitud de nivel de exposicion 
  server.on("/setUmbral", HTTP_POST, handleUmbral);

  digitalWrite(GPIO_PIN_2, LOW); // Aviso LED
}

void loop() {
  // put your main code here, to run repeatedly
  server.handleClient();
  
  if ( WL_CONNECTED != WiFi.status() ){
    Serial.print ( WiFi.status() ) ;
    //digitalWrite(GPIO_PIN_2, HIGH);
  }
}

// Calback recibir solicitudes, respuesta ack 200ok + webPage 
void handleRoot() {
  server.send(200, "text/html", webPage);
}

//Callback para actualizar el valor de la exposición
void handleExposicion() {

  //exposicionActual = (int)oLight.getPercent();
  //String exposicionValue = String(exposicionActual);
  String exposicionValue = String(rand()%101);
  
  Serial.print ("EXPOSICION:  ") ;
  Serial.println (exposicionValue) ;
  
  server.send(200, "text/plane", exposicionValue);

  //if (ok leer valor)
  //else 
  // error
}

void handleUmbral() {
  
  String umbralValue = server.arg("umbral");  // Obtener el valor del umbral desde la solicitud

  int umbralActual = umbralValue.toInt();

  Serial.print ("UMBRAL: ") ;
  Serial.println (umbralActual);

  if (exposicionActual > umbralActual){
    // Habilitar Alarma
    
  } else {
    server.send(200, "text/plane", webPage);
  }
}
