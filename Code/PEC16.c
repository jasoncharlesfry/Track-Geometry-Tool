#include "PEC16.h"
#include "tm4c123gh6pm.h"

static uint32_t PEC16_data;
//static uint32_t PEC16_A;
//static uint32_t PEC16_B;

uint8_t debug[100];
uint32_t debugIdx = 0;

int8_t stateTable[16] = {
	0,
	1,
	-1,
	0,
	-1,
	0,
	0,
	1,
	1,
	0,
	0,
	-1,
	0,
	-1,
	1,
	0
};

#define DEBOUNCE_TIME 10000

void debounce(void){
	for(volatile int i = 0; i < DEBOUNCE_TIME; i++){}
}

// prevBA concatenated with nextBA | invalid(0)/CW(1)/CCW(-1)
//----------------------------------------------
//                       0000  |  0
//                       0001  |  1
//                       0010  |  -1
//                       0011  |  0
//                       0100  |  -1
//                       0101  |  0
//                       0110  |  0
//                       0111  |  1
//                       1000  |  1
//                       1001  |  0
//                       1010  |  0
//                       1011  |  -1
//                       1100  |  0
//                       1101  |  -1
//                       1110  |  1
//                       1111  |  0

void PEC16_init(uint32_t priority){
	SYSCTL_RCGCGPIO_R     |=  0x02;         // Activate clock for Port B
	while((SYSCTL_PRGPIO_R & 0x02) != 0x02){};  // Allow time for clock to start
	GPIO_PORTB_DIR_R      &= ~0x03;         // inputs on PB0-PB1
	GPIO_PORTB_DEN_R      |=  0x03;         // enable digital port 
	
	//initialize port c as falling edge triggered interrupt
	GPIO_PORTB_IS_R &= ~0x03;     // (d) PB0-PB1 are edge-sensitive
	GPIO_PORTB_IBE_R &= ~0x03;    //     PB0-PB1 are not both edges
	GPIO_PORTB_IEV_R &= ~0x03;    //     PB0-PB1 are falling edge
	GPIO_PORTB_ICR_R = 0x03;      // (e) clear flag
	GPIO_PORTB_IM_R |= 0x03;      // (f) arm interrupt
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|(priority<<13);
	NVIC_EN0_R |= 0x00000002;      // (h) enable interrupt 2 in NVIC
		
	PEC16_data = 0;
}
	
void PEC16_set(uint32_t data){
	PEC16_data = data;
}

uint32_t PEC16_get(void){
	return PEC16_data;
}

static uint8_t prev = 0;
static uint8_t curr = 0;

uint8_t PEC16_getA(void){
	return curr & 0x01;
}

uint8_t PEC16_getB(void){
	return (GPIO_PORTB_DATA_R & 0x02) >> 1;
}




void PEC16_handler(void){
	GPIO_PORTB_ICR_R = 0x03;      // (e) clear flag
	uint32_t shiftA = 0xFFFFFF00;
	uint32_t shiftB = 0xFFFFFF00;
	debounce();
	for(int i = 0; i < 8; i++){
		//debounce();
		shiftA = (shiftA << 1) | (GPIO_PORTB_DATA_R & 0x01) | 0xFFFFFF00;
		shiftB = (shiftB << 1) | ((GPIO_PORTB_DATA_R & 0x02) >> 1) | 0xFFFFFF00;
	}
	while(!(shiftA == 0xFFFFFF00 || shiftA == 0xFFFFFFFF) || !(shiftB == 0xFFFFFF00 || shiftB == 0xFFFFFFFF)){
		//debounce();
		shiftA = (shiftA << 1) | (GPIO_PORTB_DATA_R & 0x01) | 0xFFFFFF00;
		shiftB = (shiftB << 1) | ((GPIO_PORTB_DATA_R & 0x02) >> 1) | 0xFFFFFF00;
	}
	
	curr = GPIO_PORTB_DATA_R & 0x03;
	PEC16_data += stateTable[(prev << 2) | curr];
	prev = curr;
	
	//PEC16_data += ((PEC16_A > prevA && PEC16_B == 0) || (PEC16_A < prevA && PEC16_B == 1) || (PEC16_B < prevB && PEC16_A == 0) || (PEC16_B > prevB && PEC16_A == 1)) ? 1 : -1;
	//PEC16_A = curr & 0x01;
	//PEC16_B = curr >> 1;
	/*if(debugIdx < 100){
		debug[debugIdx] = (PEC16_A << 1) | PEC16_B;
		debugIdx++;
	}*/
	
}
