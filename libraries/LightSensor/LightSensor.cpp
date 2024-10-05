#include "LightSensor.h"
#include <Arduino.h>

//Constructor
LightSensor::LightSensor(int pin, float vref, int bits) {
  my_pin = pin;
	my_vref = vref;
	my_bits = bits;
}

//Métodos

// initialize:
void LightSensor::initialize() {
	pinMode(my_pin, INPUT);
	analogReference(DEFAULT);		// tension de referencia por defecto 
	Serial.begin(9600);					// leer por terminal 
}

float LightSensor::getVoltage(){
	int codigo;
	float voltage;
	
	codigo = analogRead(my_pin);
	voltage = (float) codigo * my_vref / pow(2, my_bits);
	return voltage;
}

// getPercent: valor escalado a la intensidad de luz 0-100
float LightSensor::getPercent(){
	float voltage;
	float percent;
	
	voltage = getVoltage();
	percent = voltage * 100 / my_vref;

	return percent;
}