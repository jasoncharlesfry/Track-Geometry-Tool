#include "Sensor.h"
#include "Menu.h"
#include "VNC1L.h"
#include "Switch.h"
#include "ST7735.h"
#include "PLL.h"
#include "FixedMath.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern void DisableInterrupts(void);
extern void EnableInterrupts(void);

void measure(void);
void close(void);
void open(void);

static uint32_t measureNum = 0;
int main(void){
	PLL_Init(Bus80MHz);
	DisableInterrupts();
	ST7735_InitR(INITR_REDTAB);
	VNC1L_init();
	Sensor_init(3);
	SWITCH_init(4);
	SWITCH_setPB2Task(measure);
	//SWITCH_setPB6Task(open);
	SWITCH_setPB5Task(close);
	EnableInterrupts();
	open();
	while(1);
}

void measure(void){
	int32_t position = Sensor_getPosition();
	int32_t angle = Sensor_getAngle();
	
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	char numStr[64];
	sprintf(numStr, "Measurement %d:\n", measureNum);
	ST7735_OutString(numStr);
	VNC1L_write(numStr, strlen(numStr));
	
	ST7735_SetCursor(0, 2);
	sprintf(numStr, "Position: %d (1/32 in)", position);
	VNC1L_write(numStr, strlen(numStr));
	sprintf(numStr, "Position:\n%d", position);
	ST7735_OutString(numStr);
	
	ST7735_SetCursor(0, 5);
	VNC1L_write("\nAngle (rad): ", 14);
	ST7735_OutString("Angle (rad):\n");
	Q2_29toS(numStr, angle);
	VNC1L_write(numStr, strlen(numStr));
	ST7735_OutString(numStr);
	
	angle = rad2deg(angle);
	ST7735_SetCursor(0, 8);
	VNC1L_write("\nAngle (deg): ", 14);
	ST7735_OutString("Angle (deg):\n");
	Q8_23toS(numStr, angle);
	VNC1L_write(numStr, strlen(numStr));
	ST7735_OutString(numStr);
	
	ST7735_SetCursor(0, 11);
	VNC1L_write("\nCross Level: ", 14);
	ST7735_OutString("CrossLevel:\n");
	int32_t crossLevel= Sensor_getCrossLevel(position, angle);
	Q11_20toS(numStr, crossLevel);
	VNC1L_write(numStr, strlen(numStr));
	ST7735_OutString(numStr);
	
	VNC1L_write("\n\n", 2);
	
	measureNum++;
}

void close(void){
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	VNC1L_close("DATA.txt");
	ST7735_OutString("Disk Ejected");
}

void open(void){
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Opening File");
	VNC1L_openW("DATA.txt");
	VNC1L_seek(0);
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Ready");
}
