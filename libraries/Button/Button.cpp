#include "Button.h"
#include <Arduino.h>

//Constructor
Button::Button(int pin) {
  my_pin = pin;
}

//Métodos

// initialize: inicializa 
void Button::initialize() {
	pinMode(my_pin, INPUT_PULLUP);			// INPUT_PULLDOWN no est adefinido en Arduino.h
}

// 0 pulsado 1 not pulado (PULLUP)
int Button::buttonRead(){
	return digitalRead(my_pin);
}