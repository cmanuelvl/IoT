#include "LightSensor.h"
#include <Arduino.h>

//Constructor
LightSensor::LightSensor(float vref, int bits, int pin) {
  my_vref = vref;
	my_bits = bits;
  my_pin = pin;
}

//Metodos

// initialize: inicializa 
void LightSensor::initialize() {
	pinMode(my_pin, INPUT);
	analogReference(DEFAULT);
	Serial.begin(9600);
}

float LightSensor::getVoltage(){
	int value;
	float voltage;
	
	value = analogRead(my_pin);
	Serial.print("value: ");
  Serial.println(value); // value solo de 0 a 20.
	voltage = value * my_vref / pow(2, my_bits);
	Serial.print("voltage: ");
  Serial.println(voltage);
	return voltage;
}

int LightSensor::getScaledValue(){
	float voltage;
	int scaledValue;
	voltage = getVoltage();
	scaledValue = voltage * 6 / my_vref;
	scaledValue = round(scaledValue);
	return scaledValue;
}