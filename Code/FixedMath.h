#ifndef FIXEDMATH_H
#define FIXEDMATH_H

#include <stdint.h>

// angles are in fixed point Q2.29 since max is pi 
// values are in Q16.15 so that 32768 = 1.000000000000000 since 32768 coreseponds to 1g from the accelerometer

// x is fixed point 1.30, returns fixed point 16.15
int32_t fixedSin(int32_t x);

int32_t fixedCos(int32_t x);

int32_t fixedTan(int32_t x);

// x is fixed point 16.15, returns fixed point 1.30
int32_t fixedArcSin(int32_t x);

int32_t fixedArcCos(int32_t x);

int32_t fixedArcTan(int32_t x);

int32_t rad2deg(int32_t rad);

void Q2_29toS(char* str, int32_t x);
void Q8_23toS(char* str, int32_t x);
void Q11_20toS(char* str, int32_t x);
#endif
