#include "EV3479.h"
#include "I2C.h"
#include "tm4c123gh6pm.h"
#include "ST7735.h"

#define I2C_ADDR 0x4C

void setMode(uint8_t mode);
void setRangeCtrl(uint8_t range);
void setSampleRate(uint8_t sample_rate);
void reset(void);

void EV3479_init(void){
	//SSI_init();		// initialize ssi
	ST7735_FillScreen(ST7735_BLACK);
	ST7735_SetCursor(0, 0);
	ST7735_OutString("initializing...");
	I2C_init();
	
	reset();
	
	setMode(MC34X9_MODE_STANDBY);
		
	uint8_t ID = EV3479_read(0x18);
		
	setRangeCtrl(MC34X9_RANGE_2G);

	setSampleRate(MC34X9_SR_500Hz);
	
	EV3479_write(0x06, 0x80);


	setMode(MC34X9_MODE_CWAKE);	
	for(volatile int i = 5000000; i > 0; i--){}
		
}

#define NUM_SAMPLES 64
void EV3479_measure(int16_t *_x, int16_t *_y, int16_t *_z){
	int16_t x[NUM_SAMPLES], y[NUM_SAMPLES], z[NUM_SAMPLES];

	for(int i = 0; i < NUM_SAMPLES; i++){
		while(!(EV3479_read(0x14) & 0x80));		// wait for fresh sample
		x[i] = EV3479_read(X_LSB) | (EV3479_read(X_MSB) << 8);
		y[i] = EV3479_read(Y_LSB) | (EV3479_read(Y_MSB) << 8);
		z[i] = EV3479_read(Z_LSB) | (EV3479_read(Z_MSB) << 8);
	}
	int32_t xavg = 0;
	int32_t yavg = 0;
	int32_t zavg = 0;
	for(int i = 0; i < NUM_SAMPLES; i++){
		xavg += x[i];
		yavg += y[i];
		zavg += z[i];
	}
	
	xavg /= NUM_SAMPLES;
	yavg /= NUM_SAMPLES;
	zavg /= NUM_SAMPLES;
	
	*_x = xavg;
	*_y = yavg;
	*_z = zavg;
}

void EV3479_write(uint16_t addr, uint16_t data){
	/*GPIO_PORTD_DATA_R &= ~(0x02);	//lower cs
	SSI_out(addr);
	SSI_out(data);
	GPIO_PORTD_DATA_R |= 0x02;		// raise cs
	*/
	I2C_trans2(I2C_ADDR, addr, data);
}
	
uint8_t EV3479_read(uint16_t addr){
	/*GPIO_PORTD_DATA_R &= ~(0x02);	//lower cs
	SSI_out(0x80 | addr);
	SSI_out(0x00);
	uint16_t in =  SSI_in();
	GPIO_PORTD_DATA_R |= 0x02;		// raise cs
	return in;
	*/
	uint8_t dataIn;
	I2C_trans1_recv1(I2C_ADDR, addr, &dataIn);
	return dataIn;
}

//set mode
void setMode(uint8_t mode){
  uint8_t value;

  value = EV3479_read(MC34X9_REG_MODE);
  value &= 0xF0;
  value |= mode;

  EV3479_write(MC34X9_REG_MODE, value);
}

//Set the range control
void setRangeCtrl(uint8_t range){
  uint8_t value;
  setMode(MC34X9_MODE_STANDBY);
  value = EV3479_read(MC34X9_REG_RANGE_C);
  value &= 0x07;
  value |= (range << 4) & 0x70;
  EV3479_write(MC34X9_REG_RANGE_C, value);
}

//Set the sampling rate
void setSampleRate(uint8_t sample_rate){
  uint8_t value;
  setMode(MC34X9_MODE_STANDBY);
  value = EV3479_read(MC34X9_REG_SR);
  value &= 0x00;
  value |= sample_rate;
  EV3479_write(MC34X9_REG_SR, value);
}

void reset(){
  // Stand by mode
  EV3479_write(MC34X9_REG_MODE, MC34X9_MODE_STANDBY);
  for(volatile int i = 1000000; i > 0; i--){}
  // power-on-reset
  EV3479_write(0x1c, 0x40);
  for(volatile int i = 5000000; i > 0; i--){}
  // Disable interrupt
  EV3479_write(0x06, 0x00);
  for(volatile int i = 1000000; i > 0; i--){}
  // 1.00x Aanalog Gain
  EV3479_write(0x2B, 0x00);
  for(volatile int i = 1000000; i > 0; i--){}
  // DCM disable
  EV3479_write(0x15, 0x00);
  for(volatile int i = 5000000; i > 0; i--){}
}

