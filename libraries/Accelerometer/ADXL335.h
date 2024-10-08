#ifndef ADXL335_H
#define ADXL335_H

#include <Arduino.h>

// amplitude of the 10bit-ADC of Arduino is 1024LSB, using as default
#ifndef ADC_AMPLITUDE
#define ADC_AMPLITUDE 1024
#endif
#ifndef ADC_REF
#define ADC_REF 5  // ADC reference is 5v
#endif

// accleration of X-AXIS is 0g, the voltage of X-AXIS is 1.5v
#ifndef ZERO_X
#define ZERO_X 1.5
#endif
#ifndef ZERO_Y
#define ZERO_Y 1.5  //
#endif
#ifndef ZERO_Z
#define ZERO_Z 1.5  //
#endif

// sensitivity of X/Y/Z axis is 0.33v/g
#ifndef SENSITIVITY
#define SENSITIVITY 0.33
#endif

class ADXL335 {
	public:
		//constructor
		ADXL335(int pins[], float vref, int bits);
		
		//Métodos
		void initialize();
		void begin();
		void getXYZ(int* cx, int* cy, int* cz);
		void getOffset(float* ox, float* oy, float* oz);
		void getAngle( float* ox, float* oy, float* oz, float* x_roll, float* y_pitch);
		void getAcceleration(float* ax, float* ay, float* az);
		float getAccelerationX();
		float getAccelerationY();
		float getAccelerationZ();
	
	private:
		int my_pins[3];					//conexionado de pines
		float my_vref;
		int my_bits;
};

#endif
