  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiUDP.h>
  #include <ESP8266WebServer.h>
  #include <TimerEvent.h>
  #include "FS.h"
  #include "hardware.h"
  
  
  void medida();
  void actualizarUmbral();
  void encenderAlarma();
  const char* ssid = "HolaJuan";
  const char* password = "Castillo";
  
  ESP8266WebServer server(80);
  TimerEvent timer;
  
  String pWeb;
  int nivelLuz;
  int UMBRAL = 80;
  int prueba = 0;
  
  void setup(void) {
//  pWeb = "<!DOCTYPE html><html lang=\"es\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"><title>WEARABLE HIOT</title>";
//  pWeb += "<style> body{background-image: url('https://pymstatic.com/110684/conversions/mejores-frases-sol-wide.jpg');background-size: cover;background-position: center;height: 100vh;margin: 0;justify-content: center;align-items: center;}";
//  pWeb += ".content {background-color: rgba(255, 255, 255, 0.8);padding: 20px;border-radius: 10px;text-align: center;}</style></head>";
//  pWeb += "<body><h1 style=\"text-align: center; font-family: 'Times New Roman', Times, serif;\">CONTROLADOR DE EXPOSISCION SOLAR</h1>";
//  pWeb += "<p style=\"text-align: center; font-size: 20px; font-family: 'Times New Roman', Times, serif;\"> EXPOSICION:<input style= \"font-size: 20px; font-family: 'Times New Roman', Times, serif; border-radius: 5px;\"name=\"name\" readonly=\"readonly\" size=\"3\" type=\"text\" id = \"valorSensor\" />%</p>";
//  pWeb += "<form action=\"/umbral\" method=\"POST\" style=\"text-align: center; font-size: 20px; font-family: 'Times New Roman', Times, serif;\">UMBRAL: ";
//  pWeb += "<input style=\"text-align: center; font-size: 20px; font-family: 'Times New Roman', Times, serif; border-radius: 5px;\" size=\"5\" name=\"Vumbral\"   type=\"text\" placeholder=\"\" value=\""+ String(UMBRAL)+"\">";
//  pWeb += "<input style=\"text-align: center; font-size: 20px; font-family: 'Times New Roman', Times, serif; border-radius: 5px;\" type=\"submit\" value=\"Actualizar\"></form></br>";
//  pWeb += "<form action=\"/alarma\" method=\"POST\" style=\"text-align: center;\">";
//  pWeb += "<input style=\"text-align: center; background-color: #ff0000; font-size: 20px; color: #ffffff; border-radius: 5px; border: 2px solid #ff8888; font-family: 'Times New Roman', Times, serif;\" type=\"submit\" value=\"ALARMA\"></form>";
//  pWeb += "<script>function actualizarValor(){var xhttp = new XMLHttpRequest();xhttp.onreadystatechange = function(){if (this.readyState == 4 && this.status == 200){document.getElementById(\"valorSensor\").value = this.responseText;}};";
//  pWeb += "var esp8266IP = window.location.hostname;xhttp.open(\"GET\", \"http://\" + esp8266IP + \"/actualizar\", true);xhttp.send();}actualizarValor();setInterval(actualizarValor, 5000);</script></body></html>";
    pinMode(LED_AZUL, OUTPUT);
    pinMode(LED_ROJO, OUTPUT);
    pinMode(SENSOR_LUZ, INPUT);
    digitalWrite(LED_AZUL, 0);
    digitalWrite(LED_ROJO, 0);
    
    Serial.begin(115200);
    randomSeed( (unsigned long)( micros()%millis() ) );
    timer.set(1000, medida);

    if(!SPIFFS.begin()){
      Serial.println("Error al abrir el sistema de ficheros");
      return;
    }
    Serial.println("Sistema de archivos abierto correctamente");
    File f = SPIFFS.open("/PaginaWeb.html", "r");
    if(!f){
      Serial.println("Error al abrir el archivo pagina web");
      return;
    }
    Serial.println("Pagina Web abierto correctamente");
    //f.read((uint8_t *)&pWeb, f.size());
    while((boolean)f.available()){
      pWeb+=(char)f.read();
    }
    f.close();
    Serial.println("Archivo cerrado correctamente");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");
  
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      digitalWrite(LED_AZUL, !digitalRead(LED_AZUL));
    }
    digitalWrite(LED_AZUL, HIGH);
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    server.on("/", []() {
      server.send(200,"text/html",pWeb);
      Serial.println("Envio normal");
    });
    server.on("/alarma", HTTP_POST, encenderAlarma);
    server.on("/umbral", HTTP_POST, actualizarUmbral);
    server.on("/actualizar", []() {
      Serial.println("Envio actualizar");
      prueba = (prueba<100)? prueba +1 : 0;
      server.send(200,"text/plain",String(nivelLuz));
    });
    server.begin();
    Serial.println("HTTP server started");
  }
  
  void loop(void) {
    server.handleClient();
    timer.update();
  }

  void medida(){
    nivelLuz = analogRead(SENSOR_LUZ);
    nivelLuz = (int)nivelLuz*100/1024;
    if(nivelLuz<UMBRAL){
      digitalWrite(LED_ROJO,0);
    }
    Serial.println(nivelLuz);
  }
  void actualizarUmbral(){
    Serial.println("Envio umbral");
    if(!server.hasArg("Vumbral") || server.arg("Vumbral") == NULL){
      server.send(400,"text/plain", "400:Invalid Request");
    }else{
      pWeb.replace("name=\"Vumbral\" value = \"" + String(UMBRAL), "name=\"Vumbral\" value = \"" + server.arg("Vumbral"));
      UMBRAL = server.arg("Vumbral").toInt();
      Serial.print("Umbral: ");
      Serial.println(UMBRAL);
    }
    //Actualizar el valor de la pagina web sobre este.
    server.send(200,"text/html",pWeb);
  }
  void encenderAlarma(){
    Serial.println("Envio alarma");
    digitalWrite(LED_ROJO, HIGH);
    server.send(200,"text/html",pWeb);
  }
