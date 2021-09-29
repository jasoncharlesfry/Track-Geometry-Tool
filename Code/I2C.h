#ifndef I2C_H
#define I2C_H

#include <stdint.h>

void I2C_init(void);

uint32_t I2C_trans2(uint8_t addr, uint8_t data1, uint8_t data2);

uint32_t I2C_trans1_recv1(uint8_t addr, uint8_t dataOut, uint8_t *dataIn);

#endif
