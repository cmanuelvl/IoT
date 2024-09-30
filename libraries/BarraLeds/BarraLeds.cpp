#include "BarraLeds.h"
#include <Arduino.h>

//Constructor
BarraLeds::BarraLeds(int numLeds, int pins[]) {
	my_toggle = true;
  my_numLeds = numLeds;
  for (int i = 0; i < my_numLeds; i++) {
		my_pins[i] = pins[i];
  }
}

//Metodos

// initialize: inicializa los pines (OUTPUT) de la barra de led
void BarraLeds::initialize() {
  for (int i = 0; i < my_numLeds; i++) {
		pinMode(my_pins[i], OUTPUT);
  }
}

// ledOn:
void BarraLeds::ledOn(int index) {
  if (index >= 0 && index < my_numLeds) {
		digitalWrite(my_pins[index], HIGH);
  } else {
    Serial.println("Indice fuera de rango");
  }
}

// ledOff
void BarraLeds::ledOff(int index) {
  if (index >= 0 && index < my_numLeds) {
    digitalWrite(my_pins[index], LOW);
  } else {
    Serial.println("Índice fuera de rango");
  }
}

//ledToggle
void BarraLeds::ledToggle(int index) {
	
	digitalWrite(my_pins[index], my_toggle ? HIGH : LOW);
	my_toggle = !my_toggle;
}

//barraLedOn
void BarraLeds::barraLedOn(int nivel){
	for(int i = 0; i < my_numLeds; i++)
		ledOff(i);
	
	for(int i = 0; i <= nivel; i++)
		ledOn(i);
}