#ifndef BARRALEDS_H
#define BARRALEDS_H

class BarraLeds {
	public:
		// Constructor
		BarraLeds(int numLeds, int pins[]);

		// Metodos
		void initialize();
		void ledOn(int index);
		void ledOff(int index);

	private:
		// Variables privadas
		int my_numLeds;					//número total de Leds
		int my_pins[];					//conexionado de pines
};

#endif