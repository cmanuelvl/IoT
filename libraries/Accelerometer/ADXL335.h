#ifndef ADXL335_H
#define ADXL335_H

#include <Arduino.h>

// macro definitions of Analog read pins
#ifndef X_AXIS_PIN
#define X_AXIS_PIN A6
#endif
#ifndef Y_AXIS_PIN
#define Y_AXIS_PIN A7
#endif
#ifndef Z_AXIS_PIN
#define Z_AXIS_PIN A8
#endif

// amplitude of the 10bit-ADC of Arduino is 1024LSB, using as default
#ifndef ADC_AMPLITUDE
#define ADC_AMPLITUDE 1024
#endif
#ifndef ADC_REF
#define ADC_REF 5  // ADC reference is 5v
#endif

// accleration of X-AXIS is 0g, the voltage of X-AXIS is 1.22v
#ifndef ZERO_X
#define ZERO_X 1.22
#endif
#ifndef ZERO_Y
#define ZERO_Y 1.22  //
#endif
#ifndef ZERO_Z
#define ZERO_Z 1.25  //
#endif

// sensitivity of X/Y/Z axis is 0.25v/g
#ifndef SENSITIVITY
#define SENSITIVITY 0.25
#endif

class ADXL335 {
	public:
	//constructor
	ADXL335(int pins[], float vref, int bits);
	
	//Métodos
	void initialize();
	void begin();
	void calibration(int* x_off, int* y_off, int* z_off);
  void getXYZ(int* x, int* y, int* z);
  void getAcceleration(float* ax, float* ay, float* az);
  float getAccelerationX();
  float getAccelerationY();
  float getAccelerationZ();
	
  private:
};

#endif
