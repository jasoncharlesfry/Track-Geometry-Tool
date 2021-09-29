#include "tm4c123gh6pm.h"
#include "Switch.h"
#include "PEC16.h"
#include <stdint.h>

void GPIOPortB_Handler(void){
	if(GPIO_PORTB_RIS_R & 0x7C)
		Switch_handler();
	if(GPIO_PORTB_RIS_R & 0x03)
		PEC16_handler();
}
