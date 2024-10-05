#include "Buzzer.h"
#include <Arduino.h>

//Constructor
Buzzer::Buzzer(int pin) {
  my_pin = pin;
	my_toggle = true;
}

//Métodos

// initialize:
void Buzzer::initialize() {
	pinMode(my_pin, OUTPUT);
}

// buzzerOn:
void Buzzer::buzzerOn(int freqz){
	tone(my_pin, freqz);
}

// buzzerOff:
void Buzzer::buzzerOff(){
	noTone(my_pin);
}

// buzzerToggle: genera 2 tonos, usar timer 
void Buzzer::buzzerToggle(int freqz1, int freqz2) {
	tone(my_pin, my_toggle ? freqz1 : freqz2);
	my_toggle = !my_toggle;
}