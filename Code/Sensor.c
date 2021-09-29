#include "Sensor.h"
#include "FixedMath.h"

void Sensor_init(uint32_t priority){
	EV3479_init();
	PEC16_init(priority);
	PEC16_set(1801);
}


void Sensor_setPosition(uint32_t position){
	
}
void Sensor_setAngle(uint32_t angle);

uint32_t Sensor_getPosition(void){
	return PEC16_get();
}

//returns angle in radians as Q2.29
uint32_t Sensor_getAngle(void){
	int16_t x, y, z;
	EV3479_measure(&x, &y, &z);
	if(y > 11585 || y < -11585)
		return fixedArcCos(z);
	return fixedArcSin(y);
}


// position in 1/32 inches Q31.0, angle in radians Q2.29, return crosslevel in 1.32 in Q11.20
uint32_t Sensor_getCrossLevel(int32_t position, int32_t angle){
	volatile int32_t tan = fixedTan(angle);
	volatile int64_t crossLevel64 = (int64_t) position * tan;
	volatile int32_t crossLevel32 = crossLevel64 >> 9;
	return crossLevel32;
}
