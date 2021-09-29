#include "FixedMath.h"
#include <stdio.h>

// angles are in fixed point Q2.29 since max is pi 
// values are in Q17.14 so that 16384 = 1.0 since 16384 coreseponds to 1g from the accelerometer

// x is fixed point 1.30, returns fixed point 16.15

int32_t mulQ29 (int32_t a, int32_t b)
{
    return (int32_t)(uint32_t)((uint64_t)((int64_t)a * b) >> 29);
}

int32_t fixedSin(int32_t x);

int32_t fixedCos(int32_t x);

// input and return are Q2.29, so input angle is max 1.25 aka pi/2.5 rad aka71.5 deg
int32_t fixedTan(int32_t _x){
	int32_t x = _x < 0 ? -1*_x : _x;	// make positive
	
	
	int32_t C1 = 536870912;						// coefficients
	int32_t C3 = 178956971;
	int32_t C5 = 71582788;
	int32_t C7 = 28973986;
	int32_t C9 = 11741092;
	
	int32_t x1 = x;						// powers of x
	int32_t x3 = mulQ29(x, mulQ29(x, x));
	int32_t x5 = mulQ29(x, mulQ29(x, x3));
	int32_t x7 = mulQ29(x, mulQ29(x, x5));
	int32_t x9 = mulQ29(x, mulQ29(x, x7));
	

	int32_t T1  = mulQ29(C1, x1);
	int32_t T3  = mulQ29(C3, x3);
	int32_t T5  = mulQ29(C5, x5);
	int32_t T7  = mulQ29(C7, x7);
	int32_t T9  = mulQ29(C9, x9);
	
	int32_t tan = T1+T3+T5+T7+T9;
	
	//int32_t tan = mulQ29(C1, x1) + mulQ29(C3, x3) + mulQ29(C5, x5) + mulQ29(C7, x7) + mulQ29(C9, x9); // polynomial
	return (_x < 0 ? -1*tan : tan);		// restore sign
}

// x is fixed point 17.14, returns fixed point 2.29
int32_t fixedArcSin(int32_t _x){
	int32_t x = _x < 0 ? -1*_x : _x;	// make positive
	x = x > 16384 ? 16384 : x;			// saturate at 1
	x = x << 15;						// convert to Q2.29
	
	int32_t C1 = 536870912;						// coefficients
	int32_t C3 = 89478485;
	int32_t C5 = 40265318;
	int32_t C7 = 23967451;
	int32_t C9 = 16311182;
	
	int32_t x1 = x;						// powers of x
	int32_t x3 = mulQ29(x, mulQ29(x, x));
	int32_t x5 = mulQ29(x, mulQ29(x, x3));
	int32_t x7 = mulQ29(x, mulQ29(x, x5));
	int32_t x9 = mulQ29(x, mulQ29(x, x7));
	
	int32_t asin = mulQ29(C1, x1) + mulQ29(C3, x3) + mulQ29(C5, x5) + mulQ29(C7, x7) + mulQ29(C9, x9); // polynomial
	return (_x < 0 ? -1*asin : asin);		// restore sign
}

int32_t fixedArcCos(int32_t _x){
	//int32_t x = _x < 0 ? -1*_x : _x;	// make positive
	int32_t x = _x > 16384 ? 16384 : _x;			// saturate at 1
	x = x << 15;						// convert to Q2.29
	
	int32_t C0 = 843314857;						// coefficients
	int32_t C1 = -536870912;
	int32_t C3 = -89478485;
	int32_t C5 = -40265318;
	int32_t C7 = -23967451;
	int32_t C9 = -16311182;
	
	int32_t x1 = x;						// powers of x
	int32_t x3 = mulQ29(x, mulQ29(x, x));
	int32_t x5 = mulQ29(x, mulQ29(x, x3));
	int32_t x7 = mulQ29(x, mulQ29(x, x5));
	int32_t x9 = mulQ29(x, mulQ29(x, x7));
	
	int32_t T0 = C0;
	int32_t T1  = mulQ29(C1, x1);
	int32_t T3  = mulQ29(C3, x3);
	int32_t T5  = mulQ29(C5, x5);
	int32_t T7  = mulQ29(C7, x7);
	int32_t T9  = mulQ29(C9, x9);
	
	int32_t acos = T0+T1+T3+T5+T7+T9;
	//int32_t acos = C0 + mulQ29(C1, x1) + mulQ29(C3, x3) + mulQ29(C5, x5) + mulQ29(C7, x7) + mulQ29(C9, x9); // polynomial
	return acos;		// restore sign
}

int32_t fixedArcTan(int32_t x);

// returns degrees in Q8.23
int32_t rad2deg(int32_t _rad){
	int32_t rad = _rad < 0 ? -1*_rad : _rad;	// make positive
	int64_t convFact = 30760437400>>2; // 180/pi constant in Q36.27
	int32_t ret = (int32_t)(uint32_t)((uint64_t)((int64_t)rad * convFact) >> 33);
	return (_rad < 0 ? -1*ret : ret);
}

void Q2_29toS(char* str, int32_t x){
	int32_t integer = (x & 0x80000000 ? ((~x+1)>>29) : x >> 29) & 0x03;
	uint32_t fraction = (x & 0x80000000 ? ~(x) + 1 : x) & 0x1FFFFFFF;
	for(int i = 0; i < 9; i++){
		fraction >>= 2;
		fraction *= 5;
	}
	fraction >>= 2;
	str[0] = x & 0x80000000 ? '-' : ' ';
	sprintf(str + 1, "%u.%09u", integer, fraction);
}

void Q8_23toS(char* str, int32_t x){
	int32_t integer = (x & 0x80000000 ? ((~x+1)>>23) : x >> 23) & 0xFF;
	uint32_t fraction = (x & 0x80000000 ? ~(x) + 1 : x) & 0x007FFFFF;
	for(int i = 0; i < 7; i++){
		fraction >>= 2;
		fraction *= 5;
	}
	fraction >>= 2;
	str[0] = x & 0x80000000 ? '-' : ' ';
	sprintf(str + 1, "%u.%07u", integer, fraction);

}

void Q11_20toS(char* str, int32_t x){
	int32_t integer = (x & 0x80000000 ? ((~x+1)>>20) : x >> 20) & 0x7FF;
	uint32_t fraction = (x & 0x80000000 ? ~(x) + 1 : x) & 0x000FFFFF;
	for(int i = 0; i < 6; i++){
		fraction >>= 2;
		fraction *= 5;
	}
	fraction >>= 2;
	str[0] = x & 0x80000000 ? '-' : ' ';
	sprintf(str + 1, "%u.%06u", integer, fraction);
}


