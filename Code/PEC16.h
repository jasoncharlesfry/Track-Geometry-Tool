#ifndef PEC16_H
#define PEC16_H

#include <stdint.h>

void PEC16_init(uint32_t priority);

void PEC16_set(uint32_t data);

uint32_t PEC16_get(void);

uint8_t PEC16_getA(void);
uint8_t PEC16_getB(void);

void PEC16_handler(void);

#endif
