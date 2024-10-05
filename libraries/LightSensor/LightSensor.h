#ifndef LIGHTSENSOR_H
#define LIGHTSENSOR_H

class LightSensor {
	public:
		// Constructor
		LightSensor(int pin, float vref, int bits);

		// Métodos
		void initialize();
		float getVoltage();
		float getValue();
		float getPercent();
		
	private:
		int my_pin;					//conexión del pin
		int my_vref;				//tensión de referencia del ADC
		int my_bits;				//bits de ADC
};

#endif