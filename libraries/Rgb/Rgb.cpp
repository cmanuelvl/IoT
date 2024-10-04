#include "Rgb.h"
#include <Arduino.h>

//Constructor
Rgb::Rgb(int pins[]) {
  for (int i = 0; i < 3; i++) {
		my_pins[i] = pins[i];
  }
}

//Metodos

// initialize:
void Rgb::initialize() {
  for (int i = 0; i < 3; i++) {
		pinMode(my_pins[i], OUTPUT);
  }
}

// rgbColor: duty cycle 100% 255 0% 0
void Rgb::rgbColor(int redValue, int greenValue, int blueValue) {
    analogWrite(my_pins[0], redValue);
		analogWrite(my_pins[1], greenValue);
		analogWrite(my_pins[2], blueValue);
}
