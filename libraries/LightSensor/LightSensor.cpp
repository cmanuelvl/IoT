#include "LightSensor.h"
#include <Arduino.h>

//Constructor
LightSensor::LightSensor(int pin, float vref, int bits) {
  my_pin = pin;
	my_vref = vref;
	my_bits = bits;
}

//Métodos

// initialize: inicializa 
void LightSensor::initialize() {
	pinMode(my_pin, INPUT);
	analogReference(DEFAULT);		// tension de referencia 
	Serial.begin(9600);					// leer por terminal 
}

float LightSensor::getVoltage(){
	int value;
	float voltage;
	
	value = analogRead(my_pin);
	Serial.print("value: "); Serial.println(value); // value solo de 0 a 20.
	
	voltage = value * my_vref / pow(2, my_bits);
	Serial.print("voltage: "); Serial.println(voltage);
	return voltage;
}

// getScaledValue: valor proporcional (0-5) a la intensidad de luz
int LightSensor::getScaledValue(){
	float voltage, value;
	int scaledValue;
	voltage = getVoltage();
	value = voltage * 6 / my_vref;
	scaledValue = round(scaledValue);
	return scaledValue;
}