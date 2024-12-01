#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>  // permite conectar a la red Wifi
#include <WiFiClient.h>
#include <WiFiUDP.h>
#include <NTPClient.h>
#include <TimerEvent.h>
#include <TimeOut.h>
#include "hardware.h"
#include <LightSensor.h>
#include "FS.h"

//define
//#define VREF 3.3
#define ADC_BITS  10
#define TMR_5s 5000

//const
const char *ssid = "Juan Camilo <3";
const char *password = "camiloooo";

//Objetos
ESP8266WebServer server ( 80 );
LightSensor oLight(GPIO_PIN_ADC, 3.3, ADC_BITS);
WiFiUDP ntpUDP;
NTPClient ntpClient(ntpUDP, "0.es.pool.ntp.org", 3600,  1000);
TimerEvent tmrid_5s;

int umbralActual = 100, exposicionActual;
String webPage;
int num_medidas = 1;
int acumulado = 0; 
  
void setup() {
  Serial.begin ( 115200 );

  delay (1000);
  if(!SPIFFS.begin()){
      Serial.println("Error al abrir el sistema de ficheros");
      return;
    }
    delay (1000);
    Serial.println("Sistema de archivos abierto correctamente");
    File f = SPIFFS.open("/servidorExposicion.html", "r");
    if(!f){
      Serial.println("Error al abrir el archivo pagina web");
      return;
    }
    Serial.println("Pagina Web abierta correctamente.");
  
    //f.read((uint8_t *)&pWeb, f.size());
    while((boolean)f.available()){
      webPage+=(char)f.read();
    }
    f.close();
    Serial.println("Archivo cerrado correctamente");
  
  // Init
  oLight.initialize();
  pinMode(GPIO_PIN_2, OUTPUT);
  umbralActual = 100;
  tmrid_5s.set(TMR_5s, tmr_Callback_getMedida);
  
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
  ntpClient.begin();
  server.begin();
  server.on("/", handleRoot);   // escuchando solicitud de pagina 
  server.on("/readExposicion", handleExposicion); // escuchando solicitud de nivel de exposicion 
  server.on("/setUmbral", HTTP_POST, handleUmbral);
  server.on("/activarAlarma", HTTP_POST, handleActivarAlarma); // esuchando solicitud de activación de alarma 

  digitalWrite(GPIO_PIN_2, LOW); // Aviso LED
}

void loop() {
  // put your main code here, to run repeatedly
  ntpClient.update();
  server.handleClient();
  tmrid_5s.update();
  
  if ( WL_CONNECTED != WiFi.status() ){
    Serial.print ( WiFi.status() ) ;
    //digitalWrite(GPIO_PIN_2, HIGH);
  }
}

// Calback para tomar medidas
void tmr_Callback_getMedida(){
  String aux;
  
  //exposicionActual = (int)oLight.getPercent();
  //String exposicionValue = String(exposicionActual);
  int exposicionValue = (int)(rand()%101);
  Serial.println(ntpClient.getFormattedTime()+"-> Nivel de luz: "+exposicionValue + " -> nº medida: "+num_medidas);
  acumulado = acumulado + exposicionValue;
  Serial.println("Valor acumulador: " + (String)acumulado);

  // Escribir las medidas
    if(num_medidas == 12){
      File f = SPIFFS.open("/FicheroMedidas.txt","a+");//mantiene valores antiguos y añade el nuevo al principio del documento
      if(!f){
        Serial.println("No se puede abrir el fichero de medidas");
        return;
      }
      if(acumulado/num_medidas > umbralActual){
        aux = ntpClient.getFormattedTime()+ ": " + "Media -> " + (String)(acumulado/num_medidas) + ".Se ha superado el umbral." + "\n";
      }else{
        aux = ntpClient.getFormattedTime()+ ": " + "Media -> " + (String)(acumulado/num_medidas) + ".No se ha superado el umbral." + "\n";
      }
      if (f.print(aux)) {
       Serial.println("Archivo escrito correctamente");
      } else {
       Serial.println("Error al escribir en el archivo");
      }
      f.close();
    }
    
    num_medidas++;   
    if(num_medidas > 12){
      num_medidas = 1;
      acumulado = 0;
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

void handleActivarAlarma() {

  Serial.println("Alarma activada!");
  server.send(200, "text/plain", "Alarma activada");
}
