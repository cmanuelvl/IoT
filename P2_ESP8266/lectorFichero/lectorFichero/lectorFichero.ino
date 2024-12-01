  #include "FS.h"

void setup() {
    Serial.begin(115200);
    delay(5000);
    if(!SPIFFS.begin()){
      Serial.println("Error al abrir el sistema de ficheros");
      return;
    }
    File f = SPIFFS.open("/FicheroMedidas.txt","r");

    String lastValue = "";
    if (f){
      while (f.available()) {
        Serial.print(f.readString());
      }
    Serial.println(lastValue);
    Serial.println("Fin de fichero");
    }else{
      Serial.println("Fichero no abierto");
    }
}

void loop() {
  // put your main code here, to run repeatedly:

}
