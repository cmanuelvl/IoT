#ifndef BARRALEDS_H
#define BARRALEDS_H

class BarraLeds {
	public:
		// Constructor
		BarraLeds(int pins[], int numLeds);

		// Métodos
		void initialize();
		void ledOn(int index);
		void ledOff(int index);
		void ledToggle(int index);
		void barraLedOn(int nivel);
		
	private:
		int my_pins[6];					//conexionado de pines
		int my_numLeds;					//número total de Leds
		bool my_toggle;					//boolean para hacer toggle
};

#endif