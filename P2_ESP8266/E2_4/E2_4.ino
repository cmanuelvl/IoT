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
const char *ssid = "MiFibra-VJZ";
const char *password = "V51748155H";

//Objetos
ESP8266WebServer server ( 80 );
LightSensor oLight(GPIO_PIN_ADC, 3.3, ADC_BITS);
TimerEvent tmrid_500ms;

int umbralActual = 100, exposicionActual;
String webPage;
  
void setup() {
  Serial.begin ( 115200 );
  
/***************************************************** WEBPAGE ******************************************************************************/
webPage = "<html><head><title>Exposicion Luminica</title>"
          "<style>body {text-align: center;}</style></head>"
          "<body><h1>EXPOSICION LUMINICA</h1>"
          "<p>Exposicion: <span id=\"exposicionValue\"></span>%</p>"
          "<form><label for=\"umbral\">Umbral:</label>"
          "<input type=\"number\" id=\"umbral\" name=\"umbral\" required min=\"0\" max=\"100\">"
          "<button type=\"submit\">Enviar</button></form>"
          "<div id=\"alarma\" style=\"display: none;\">"
          "<p><strong>Luminosidad alta!</strong></p>"
          "<button id=\"activarAlarma\" onclick=\"activarAlarma()\">Activar Alarma</button></div>"
          "<script>"
          "let umbral_ini = " + String(umbralActual) + "; "
          "function getExposicionData() {"
          "fetch('/readExposicion').then(response => response.text()).then(data => {"
          "document.getElementById('exposicionValue').textContent = data;"
          "let exposicion = parseInt(data, 10);"
          "let umbral = parseInt(document.getElementById('umbral').value, 10) || umbral_ini;"
          "if (exposicion > umbral) {"
          "document.getElementById('alarma').style.display = 'block';"
          "} else {"
          "document.getElementById('alarma').style.display = 'none';"
          "}"
          "}).catch(error => {console.error('Error al obtener los datos:', error);});"
          "}"
          "document.querySelector('form').addEventListener('submit', (event) => {"
          "event.preventDefault();"
          "const umbral = document.getElementById('umbral').value;"
          "fetch('/setUmbral', {"
          "method: 'POST',"
          "headers: {'Content-Type': 'application/x-www-form-urlencoded'},"
          "body: `umbral=${umbral}`"
          "})"
          ".then(response => {"
          "if (response.ok) {"
          "console.log('Umbral establecido correctamente');"
          "document.getElementById('umbralValue').textContent = umbral;"
          "} else {"
          "console.error('Error al establecer el umbral');"
          "}"
          "})"
          ".catch(error => {console.error('Error al enviar el formulario:', error);});"
          "});"
          "getExposicionData();"
          "setInterval(getExposicionData, 1000);"
          "</script></body></html>";
/********************************************************************************************************************************************/
  
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

  umbralActual = umbralValue.toInt();

  Serial.print ("UMBRAL: ") ;
  Serial.println (umbralActual);

  if (exposicionActual > umbralActual){
    // Habilitar Alarma
    
  } else {
    server.send(200, "text/plane", webPage);
  }
}
