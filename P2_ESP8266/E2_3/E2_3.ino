#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
//#include <ESP8266mDNS.h>

const char *ssid = " ";
const char *password = "              ";

ESP8266WebServer server ( 80 );

//const int ledl = 5;
//const int led2 = 14;

String webPage;

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
