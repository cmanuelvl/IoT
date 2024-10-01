#ifndef BUZZER_H
#define BUZZER_H

class Buzzer {
	public:
		// Constructor
		Buzzer(int pin);

		// Metodos
		void initialize();
		void buzzerOn(int freqz);
		void buzzerOff();
		void buzzerToggle(int freqz1, int freqz2);
		
	private:
		int my_pin;
		bool my_toggle;
};

#endif