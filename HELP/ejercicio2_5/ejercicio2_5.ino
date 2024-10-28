  #include <ESP8266WiFi.h>
  #include <WiFiClient.h>
  #include <WiFiUDP.h>
  #include <ESP8266WebServer.h>
  #include <NTPClient.h>
  #include "FS.h"
  #include "hardware.h"
  #include <TimerEvent.h>
  
  
  void medida();
  void actualizarUmbral();
  void encenderAlarma();
  const char* ssid = "HolaJuan";
  const char* password = "Castillo";
  //const char* ssid = "XiaomiPad5";
  //const char* password = "12345678";
  
  ESP8266WebServer server(80);
  WiFiUDP ntp;
  NTPClient ntpClient(ntp, "0.es.pool.ntp.org", 3600,  1000);
  TimerEvent timer;
  
  String pWeb;
  int nivelLuz;
  int UMBRAL = 80;
  int prueba = 0;
  int seconds = 0;
  int num_medidas = 1;
  int acum_medidas = 0;
  
  void setup(void) {
    pinMode(LED_AZUL, OUTPUT);
    pinMode(LED_ROJO, OUTPUT);
    pinMode(SENSOR_LUZ, INPUT);
    digitalWrite(LED_AZUL, 0);
    digitalWrite(LED_ROJO, 0);
    
    Serial.begin(115200);
    randomSeed( (unsigned long)( micros()%millis() ) );
    timer.set(5000, medida);

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
    Serial.println("Pagina Web abierto correctamente.");
  
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
    ntpClient.begin();
    server.on("/", []() {
      server.send(200,"text/html",pWeb);
      Serial.println("Envio normal");
    });
    
    server.on("/alarma", HTTP_POST, encenderAlarma);
    
    server.on("/umbral", HTTP_POST, actualizarUmbral);
    
    server.on("/actualizar", []() {
      //Serial.println("Envio actualizar");
      prueba = (prueba<100)? prueba +1 : 0;
      server.send(200,"text/plain",String(nivelLuz));
    });
    
    server.begin();
    Serial.println("HTTP server started");
  }
  
  void loop(void) {
    server.handleClient();
    ntpClient.update();
    timer.update();
  }

  void medida(){
    nivelLuz = analogRead(SENSOR_LUZ);
    nivelLuz = (int)nivelLuz*100/1024;
    Serial.println(ntpClient.getFormattedTime()+"-> Nivel de luz: "+nivelLuz + " -> nº medida: "+num_medidas);
    acum_medidas = acum_medidas + nivelLuz;
    Serial.println("Valor acumulador: " + (String)acum_medidas);
    
    String aux;

    if(nivelLuz<UMBRAL){
      digitalWrite(LED_ROJO,0);
    }

    //ESCRITURA
    if(num_medidas == 12){
      File f = SPIFFS.open("/FicheroMedidas.txt","a+");//mantiene valores antiguos y añade el nuevo al principio del documento
      if(!f){
        Serial.println("No se puede abrir el fichero de medidas");
        return;
      }
      if(acum_medidas/num_medidas > UMBRAL){
        aux = ntpClient.getFormattedTime()+ ": " + "Media -> " + (String)(acum_medidas/num_medidas) + ".Se ha superado el umbral." + "\n";
      }else{
        aux = ntpClient.getFormattedTime()+ ": " + "Media -> " + (String)(acum_medidas/num_medidas) + ".No se ha superado el umbral." + "\n";
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
      acum_medidas = 0;
    }
  }
  void actualizarUmbral(){
    //Serial.println("Envio umbral");
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
    //Serial.println("Envio alarma");
    digitalWrite(LED_ROJO, HIGH);
    server.send(200,"text/html",pWeb);
  }
