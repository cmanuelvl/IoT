#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensor {
	public:
		// Constructor
		LightSensor(float vref, int bits, int pin);

		// Metodos
		void initialize();
		float getVoltage();
		float getValue();
		int getScaledValue();
		
	private:
		// Variables privadas
		int my_vref;					//tensión de referencia del ADC
		int my_bits;
		int my_pin;					//conexión del pin

};

#endif