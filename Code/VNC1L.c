#include "VNC1l.h"
#include "UART.h"
#include <string.h>
#include "ST7735.h"
#include "tm4c123gh6pm.h"

void VNC1L_init(void){
	UART_Init();
	SYSCTL_RCGCGPIO_R |= 0x10;            	// activate port E
	while((SYSCTL_PRGPIO_R & 0x10) == 0){}	//wait for clock
	GPIO_PORTE_DIR_R |= 0x01;				// PE0 output (CTS)
	GPIO_PORTE_DEN_R |= 0x01;				// PE0 enabled
	GPIO_PORTE_DATA_R &= ~0x01;				// bring CTS low
	char buff[33];
	UART_InString(buff, 32);				// inital newline
	ST7735_SetCursor(0, 0);
	ST7735_OutString(buff);
	UART_InString(buff, 32);				// firmware description
	ST7735_SetCursor(0, 1);
	ST7735_OutString(buff);
	UART_InString(buff, 32);				// device detected
	ST7735_SetCursor(0, 2);
	ST7735_OutString(buff);
	UART_InString(buff, 32);				// no upgrade
	ST7735_SetCursor(0, 3);
	ST7735_OutString(buff);
	UART_InString(buff, 32);				// prompt
	ST7735_SetCursor(0, 4);
	ST7735_OutString(buff);
}

void VNC1L_command(uint32_t cmd, char* str1, char *str2, char *buf, int size){
	UART_OutChar(cmd);
	if(size >= 0){
		UART_OutChar(PRE);
		UART_OutUDec((uint32_t)size);
	}
	if(str1 != 0){
		UART_OutChar(PRE);
		UART_OutString(str1);
	}
	if(str2 != 0){
		UART_OutChar(PRE);
		UART_OutString(str2);
	}
	UART_OutChar(END);
	if(buf != 0){
		UART_InString(buf, size);
	}
}

void VNC1L_ls(char *buff, uint32_t size){
	UART_OutString("DIR");
	UART_OutChar(END);
	char inString[64];
	if(size > 0){
		buff[0] = '\0';
	}
	UART_InString(inString, 64);
	while(strcmp(inString, PROMPT) != 0){
		strlcat(buff, ",", size);
		strlcat(buff, inString, size);
		UART_InString(inString, 64);
	}
}
void VNC1L_cd(char* dir){
	UART_OutString("CD ");
	UART_OutString(dir);
	UART_OutChar(END);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}
void VNC1L_openR(char* file){
	UART_OutString("OPR ");
	UART_OutString(file);
	UART_OutChar(END);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}

void VNC1L_openW(char* file){
	UART_OutString("OPW ");
	UART_OutString(file);
	UART_OutChar(END);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}

void VNC1L_read(char *buff, uint32_t size){
	UART_OutString("RDF ");
	
	UART_OutChar(((size & 0xFF000000) >> 24));
	UART_OutChar(((size & 0x00FF0000) >> 16));
	UART_OutChar(((size & 0x0000FF00) >> 8));
	UART_OutChar(((size & 0x000000FF) >> 0));
	
	UART_OutChar(END);
	UART_InString(buff, size);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}

void VNC1L_write(char* buff, uint32_t size){
	UART_OutString("WRF ");
	UART_OutChar(((size & 0xFF000000) >> 24));
	UART_OutChar(((size & 0x00FF0000) >> 16));
	UART_OutChar(((size & 0x0000FF00) >> 8));
	UART_OutChar(((size & 0x000000FF) >> 0));
	UART_OutChar(END);
	UART_OutString(buff);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}

void VNC1L_seek(uint32_t pos){
	UART_OutString("SEK ");
	
	UART_OutChar(((pos & 0xFF000000) >> 24));
	UART_OutChar(((pos & 0x00FF0000) >> 16));
	UART_OutChar(((pos & 0x0000FF00) >> 8));
	UART_OutChar(((pos & 0x000000FF) >> 0));
	
	UART_OutChar(END);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}

void VNC1L_close(char* file){
	UART_OutString("CLF ");
	UART_OutString(file);
	UART_OutChar(END);
	
	char inString[64];
	UART_InString(inString, 64);	// eat prompt
}
