#include "Buzzer.h"
#include <Arduino.h>

//Constructor
Buzzer::Buzzer(int pin) {
  my_pin = pin;
	my_toggle = true;
}

//Métodos

// initialize: inicializa 
void Buzzer::initialize() {
	pinMode(my_pin, OUTPUT);
}

void Buzzer::buzzerOn(int freqz){
	tone(my_pin, freqz);
}

void Buzzer::buzzerOff(){
	noTone(my_pin);
}

// buzzerToggle:
void Buzzer::buzzerToggle(int freqz1, int freqz2) {
	tone(my_pin, my_toggle ? freqz1 : freqz2);
	my_toggle = !my_toggle;
}