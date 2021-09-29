#ifndef SENSOR_H
#define SENSOR_H

#include <stdint.h>
#include "PEC16.h"
#include "EV3479.h"

void Sensor_init(uint32_t priority);

void Sensor_setPosition(uint32_t position);
void Sensor_setAngle(uint32_t angle);

uint32_t Sensor_getPosition(void);
uint32_t Sensor_getAngle(void);

uint32_t Sensor_getCrossLevel(int32_t position, int32_t angle);

#endif
