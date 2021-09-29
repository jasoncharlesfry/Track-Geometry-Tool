#ifndef __SWITCH_H__
#define __SWITCH_H__
#include <stdint.h>

#define DEBOUNCE_TIME 50000

void SWITCH_init(uint32_t priority);

void SWITCH_debounce(void);

void SWITCH_setPB2Task(void(*task)(void));
void SWITCH_setPB3Task(void(*task)(void));
void SWITCH_setPB4Task(void(*task)(void));
void SWITCH_setPB5Task(void(*task)(void));
void SWITCH_setPB6Task(void(*task)(void));

void Switch_handler(void);


#endif
