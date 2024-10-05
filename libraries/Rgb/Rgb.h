#ifndef RGB_H
#define RGB_H

class Rgb {
	public:
		// Constructor
		Rgb(int pins[]);

		// Métodos
		void initialize();
		void rgbColor(int redValue, int greenValue, int blueValue);
		
	private:
		int my_pins[3];					//conexionado de pines
};

#endif
