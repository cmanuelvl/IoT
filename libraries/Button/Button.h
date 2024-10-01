#ifndef BUTTON_H
#define BUTTON_H

class Button {
	public:
		// Constructor
		Button(int pin);

		// Metodos
		void initialize();
		int buttonRead();
		
	private:
		int my_pin;
};

#endif