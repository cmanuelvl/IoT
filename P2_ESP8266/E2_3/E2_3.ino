#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include <ESP8266mDNS.h>

#include <Buzzer.h>
#include <hardware.h>
#include <TimeOut.h>

// define
#define TMR_1s   1000
#define TMR_2s   2000

// Objetos 
Buzzer oBuzzer(PIN_BUZZER);
TimeOut tmrid_1s;
TimeOut tmrid_2s;

// Typedef
typedef enum {NORMAL, ALARMA_RGB, ALARMA_BUZZER, WAIT} t_estados;
t_estados estado;

// Variables 
bool buzzerOn;
int cuenta = 0; // Para contar las tres rafagas del altavoz 

const char *ssid = " ";
const char *password = "              ";

ESP8266WebServer server ( 80 );

//const int ledl = 5;
//const int led2 = 14;

String webPage;

// Function prototype 
void paginaNoEncontrada();
void Led_Blue_Init();
void tmr_Callback_1s();
void tmr_Callback_2s();


// Funcion que proporciona un mensaje detallado cuando se acceda a una ruta no encontrada en el servidor web del ESP8266
void paginaNoEncontrada() {

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args ( ) ;
  message += "\n";

  // Iterar sobre los argumentos y añadirlos al mensaje
  for ( uint8_t i = 0; i < server.args() ; i++ ) {
  message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  // Enviar respuesta 404 con el mensaje
  server.send ( 404, "text/plain", message );
}

void setup() {
  // put your setup code here, to run once:
//  pinMode ( ledl, OUTPUT );
//  pinMode ( led2, OUTPUT );
//  digitalWrite ( ledl, 1);
//  digitalWrite ( led2, 1);

  // Inicializacion perifericos 
  Led_Blue_Init();
  obuzzer.initialize();

  // Inicializacion WIFI
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi. status () != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }
  Serial.println ( "" );
  Serial.print ( "Connected to " ) ;
  Serial.println ( ssid );
  Serial.print ( "IP address: " ) ;
  Serial.println ( WiFi. localIP() ) ;

  digitalWrite(PIN_LED_B, HIGH); // Encender lez azul para notificar la conexion 
  buzzerOn = true; // Encender Altavoz 
  oBuzzer.buzzerToggle(350, 500);
  tmrid_1s.timeOut(TMR_1s, tmr_Callback_1s); 
  
  //digitalWrite ( ledl, 0);

  webPage= "<p style=\"padding-left: 120px;\"><span style=\"text-decoration: underline; color: #ff0000;\"><strong>ESTE ES MI SERVER</strong></span></p>";
  webPage += "<p style=\"padding-left: 120px;\">&nbsp ;< /p><p>anbsp ;< a href=\"On\">Pulsa Enceder</a></p>";
  webPage += "<p>&nbsp ;< /p><p><a href=\"Off\">Pulsa Apagar</a></p>";

  server.on("/", [](){
    server.send(200, "text/html", webPage); // Envio del archivo html pidiendo contrasena
  });

  server.on("/On", [](){
    //digitalWrite ( led2, 0);
    server. send(200, "text/html", webPage); // Envio del archivo html pidiendo contraseña
  });

  server.on("/Off", [](){
    //digitalWrite ( led2, 1);
    digitalWrite(PIN_LED_B, LOW); // Apagar lez azul para notificar la desconexion
    server.send(200, "text/html", webPage); // Envio del archivo html pidiendo contrasena
  });

  server.onNotFound ( paginaNoEncontrada );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient(); // Procesa las solicitudes HTTP de los clientes 
}

// Inicializacion Led Blue en modo Salida
void Led_Blue_Init(){
  pinMode(PIN_LED_B, OUTPUT);
}

void tmr_Callback_1s(){
  obuzzer.BuzzerOff(); 
  tmrid_2s.timeOut(TMR_2s, tmr_Callback_2s); 
}

void tmr_Callback_2s(){
  buzzerOn = true;
  oBuzzer.buzzerToggle(350, 500);
  cuenta = cuenta + 1;
  if (cuenta < 3){
    tmrid_1s.timeOut(TMR_1s, tmr_Callback_1s);
  }else { // cuenta == 3
    obuzzer.BuzzerOff(); 
  }
   
}
