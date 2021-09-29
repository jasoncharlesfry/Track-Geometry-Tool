#include "Switch.h"
#include "tm4c123gh6pm.h"

#include <stdint.h>

void foo(void){}

void (*PB2Task)(void) = foo;
void (*PB3Task)(void) = foo;
void (*PB4Task)(void) = foo;
void (*PB5Task)(void) = foo;
void (*PB6Task)(void) = foo;

void SWITCH_init(uint32_t priority){
	
	SYSCTL_RCGCGPIO_R     |=  0x02;         // Activate clock for Port B
	while((SYSCTL_PRGPIO_R & 0x02) != 0x02){};  // Allow time for clock to start

	GPIO_PORTB_PCTL_R     &= ~0xFFFF0000;   // regular GPIO
	GPIO_PORTB_AMSEL_R    &= ~0x7C;         // disable analog function 
	GPIO_PORTB_DIR_R      &= ~0x7C;         // inputs
	GPIO_PORTB_AFSEL_R    &= ~0x7C;         // regular port function
	GPIO_PORTB_PDR_R       =  0x7C;         // enable pull-down
	GPIO_PORTB_DEN_R      |=  0x7C;         // enable digital port 
	
	//initialize port c as falling edge triggered interrupt
	GPIO_PORTB_IS_R &= ~0x7C;     // (d) edge-sensitive
	GPIO_PORTB_IBE_R &= ~0x7C;    //     not both edges
	GPIO_PORTB_IEV_R |= 0x7C;    //     rising edge
	GPIO_PORTB_ICR_R = 0x7C;      // (e) clear flag
	GPIO_PORTB_IM_R |= 0x7C;      // (f) arm interrupt
	NVIC_PRI0_R = (NVIC_PRI0_R&0xFFFF1FFF)|(priority<<13);
	NVIC_EN0_R |= 0x00000002;      // (h) enable interrupt 1 in NVIC
}


void SWITCH_debounce(void){
	for(volatile int i = 0; i < DEBOUNCE_TIME; i++){}
}

void SWITCH_setPB2Task(void(*task)(void)){
	if(task == 0)
		PB2Task = foo;
	else
		PB2Task = task;
}

void SWITCH_setPB3Task(void(*task)(void)){
	if(task == 0)
		PB3Task = foo;
	else
		PB3Task = task;
}

void SWITCH_setPB4Task(void(*task)(void)){
	if(task == 0)
		PB4Task = foo;
	else
		PB4Task = task;
}

void SWITCH_setPB5Task(void(*task)(void)){
	if(task == 0)
		PB5Task = foo;
	else
		PB5Task = task;
}

void SWITCH_setPB6Task(void(*task)(void)){
	if(task == 0)
		PB6Task = foo;
	else
		PB6Task = task;
}


void Switch_handler(void){
	if(GPIO_PORTB_RIS_R & 0x04){ //PB2 (center)
		SWITCH_debounce();
		while(!(GPIO_PORTB_DATA_R & 0x04)){}
		SWITCH_debounce();
		(*PB2Task)();	
		GPIO_PORTB_ICR_R = 0x04;      // acknowledge flag
	}
	if(GPIO_PORTB_RIS_R & 0x08){ //PB3 (up)
		SWITCH_debounce();
		while(!(GPIO_PORTB_DATA_R & 0x08)){}
		SWITCH_debounce();
		(*PB3Task)();	
		GPIO_PORTB_ICR_R = 0x08;      // acknowledge flag
	}
	if(GPIO_PORTB_RIS_R & 0x10){ //PB4 (down)
		SWITCH_debounce();
		while(!(GPIO_PORTB_DATA_R & 0x10)){}
		SWITCH_debounce();
		(*PB4Task)();	
		GPIO_PORTB_ICR_R = 0x10;      // acknowledge flag
	}
	if(GPIO_PORTB_RIS_R & 0x20){ //PB5 (left)
		SWITCH_debounce();
		while(!(GPIO_PORTB_DATA_R & 0x20)){}
		SWITCH_debounce();
		(*PB5Task)();	
		GPIO_PORTB_ICR_R = 0x20;      // acknowledge flag
	}
	if(GPIO_PORTB_RIS_R & 0x40){ //PB6 (right)
		SWITCH_debounce();
		while(!(GPIO_PORTB_DATA_R & 0x40)){}
		SWITCH_debounce();
		(*PB6Task)();	
		GPIO_PORTB_ICR_R = 0x40;      // acknowledge flag
	}
}
