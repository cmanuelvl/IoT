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
		void ledToggle(int index);
		void barraLedOn(int nivel);
	private:
		// Variables privadas
		int my_numLeds;					//número total de Leds
		int my_pins[6];					//conexionado de pines
		bool my_toggle;					//boleano para hacer toggle
};

#endif