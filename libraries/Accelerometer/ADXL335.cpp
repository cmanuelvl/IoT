#include "ADXL335.h"

// Constructor
ADXL335::ADXL335(int pins[], float vref, int bits) {
	for (int i = 0; i < 3; i++) {
		my_pins[i] = pins[i];
  }
	my_vref = vref;
	my_bits = bits;
}

// initialize:
void ADXL335::initialize() {
   for (int i = 0; i < 3; i++) {
		pinMode(my_pins[i], INPUT);
  }
}

//void ADXL335::begin() {
//    scale = (float)SENSITIVITY * ADC_AMPLITUDE / ADC_REF;
//}

// getXYZ: obtiene el CODIGO del adc
void ADXL335::getXYZ(int* cx, int* cy, int* cz) {
    *cx = analogRead(my_pins[0]);
    *cy = analogRead(my_pins[1]);
    *cz = analogRead(my_pins[2]);
}

// getOffset: offset del adc 
void ADXL335::getOffset(float* ox, float* oy, float* oz){
	
	float sum_x = 0;
	float	sum_y = 0;
	float sum_z = 0;
	
	for (int i = 0; i < 100; i++) {
		sum_x += analogRead(my_pins[0]);
    sum_y += analogRead(my_pins[1]);
    sum_z += analogRead(my_pins[2]);
		
		delay(50);		
	}
	
	*ox = sum_x / 100;
  *oy = sum_y / 100;
  *oz = sum_z / 100;
} 

void ADXL335::getAngle( float* ox, float* oy, float* oz, float* x_roll, float* y_pitch){
	int cx, cy, cz;
	
	getXYZ(&cx, &cy, &cz);
	
	int ax = (int)((cx - *ox) * my_vref / pow(my_bits, 2)) / SENSITIVITY;
	int ay = (int)((cy - *oy) * my_vref / pow(my_bits, 2)) / SENSITIVITY;
  int az = (int)(((cz - *oz) * my_vref / pow(my_bits, 2)) / SENSITIVITY);
	
	//Serial.print(ax); Serial.print(":"); Serial.print(ay);  Serial.print(":"); Serial.print(az);  Serial.print("g ");
	//Serial.print(atan2(ay, sqrt(ax*ax+az*az))); Serial.print(" rad: "); Serial.print(ax, sqrt(ay*ay+az*az));  Serial.print(" rad: ");
	
	*x_roll  = atan2(ay, sqrt(ax*ax + az*az)) * 180/PI;
	*y_pitch = atan2(ax, sqrt(ay*ay + az*az)) * 180/PI;
}

void ADXL335::getAcceleration(float* ax, float* ay, float* az) {
    int x, y, z;
    float xvoltage, yvoltage, zvoltage;
    getXYZ(&x, &y, &z);
    
    xvoltage = (float)x * ADC_REF / ADC_AMPLITUDE;
    yvoltage = (float)y * ADC_REF / ADC_AMPLITUDE;
    zvoltage = (float)z * ADC_REF / ADC_AMPLITUDE;
		
		Serial.print(xvoltage); Serial.print(":"); Serial.print(yvoltage);  Serial.print(":"); Serial.print(zvoltage);  Serial.print("V ");
    
    *ax = (xvoltage - ZERO_X) / SENSITIVITY;
    *ay = (yvoltage - ZERO_Y) / SENSITIVITY;
    *az = (zvoltage - ZERO_Z) / SENSITIVITY;
}

float ADXL335::getAccelerationX() {
    int x, y, z;
    float xvoltage, ax;
    getXYZ(&x, &y, &z);
    xvoltage = (float)x * ADC_REF / ADC_AMPLITUDE;
    ax = (xvoltage - ZERO_X) / SENSITIVITY;
    return ax;
}

float ADXL335::getAccelerationY() {
    int x, y, z;
    float yvoltage, ay;
    getXYZ(&x, &y, &z);
    yvoltage = (float)y * ADC_REF / ADC_AMPLITUDE;
    ay = (yvoltage - ZERO_Y) / SENSITIVITY;
    return ay;
}

float ADXL335::getAccelerationZ() {
    int x, y, z;
    float zvoltage, az;
    getXYZ(&x, &y, &z);
    zvoltage = (float)z * ADC_REF / ADC_AMPLITUDE;
    az = (zvoltage - ZERO_Z) / SENSITIVITY;
    return az;
}

// Función que detecta si el brazo ha sido levantado
//bool isArmLifted(float ax, float ay, float az) {
  // Detecta el levantamiento si:
  // 1. La aceleración en Z disminuye (brazo moviéndose de vertical a horizontal)
  // 2. Aceleraciones significativas en X o Y (debido al cambio de orientación)
  
  // El brazo se levanta cuando la aceleración en Z disminuye significativamente y hay un cambio en X o Y
  //if (az < 0.5 && (abs(ax) > threshold || abs(ay) > threshold)) {
  //  return true;
  //}
  
  //return false;
//}
