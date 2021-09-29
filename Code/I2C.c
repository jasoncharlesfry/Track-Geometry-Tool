#include "I2C.h"
#include "tm4c123gh6pm.h"

// initialize I2C3 with PD0 as SCL and PD1 as SDA
void I2C_init(void){
	///*
	SYSCTL_RCGCI2C_R |= 0x08;				// activate I2C3
	SYSCTL_RCGCGPIO_R |= 0x08;				// activate port D
	while((SYSCTL_PRGPIO_R & 0x08) == 0){}	// wait for clocks
	GPIO_PORTD_AFSEL_R |= 0x03;				// enable alt func on PD0,1
	GPIO_PORTD_ODR_R |= 0x02;				// enable open drain on SDA (PD1)
	GPIO_PORTD_DEN_R |= 0x03;				// digital enable
	GPIO_PORTD_PCTL_R = (GPIO_PORTD_PCTL_R & 0xFFFFFF00) | 0x33;	// select I2C alternate function for PD0,1
	GPIO_PORTD_AMSEL_R &= ~0x03;          // 7) disable analog functionality on PB2,3
	I2C3_MCR_R = I2C_MCR_MFE;					// master mode;
	I2C3_MTPR_R = 39;						// set speed to 100 kHz for 80 MHz bus
	//*/
	/*
	SYSCTL_RCGCI2C_R |= 0x0001;           // activate I2C0
	SYSCTL_RCGCGPIO_R |= 0x0002;          // activate port B
	while((SYSCTL_PRGPIO_R&0x0002) == 0){};// ready?

	GPIO_PORTB_AFSEL_R |= 0x0C;           // 3) enable alt funct on PB2,3
	GPIO_PORTB_ODR_R |= 0x08;             // 4) enable open drain on PB3 only
	GPIO_PORTB_DEN_R |= 0x0C;             // 5) enable digital I/O on PB2,3
										// 6) configure PB2,3 as I2C
	GPIO_PORTB_PCTL_R = (GPIO_PORTB_PCTL_R&0xFFFF00FF)+0x00003300;
	GPIO_PORTB_AMSEL_R &= ~0x0C;          // 7) disable analog functionality on PB2,3
	I2C0_MCR_R = I2C_MCR_MFE;      // 9) master function enable
	I2C0_MTPR_R = 39;              // 8) configure for 100 kbps clock
	*/
}

uint32_t I2C_trans2(uint8_t addr, uint8_t data1, uint8_t data2){
	///*
  while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C3_MSA_R = (addr<<1)&0xFE;    // MSA[7:1] is slave address
  I2C3_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C3_MDR_R = data1&0xFF;         // prepare first byte
  I2C3_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                    //   & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // check error bits
  if((I2C3_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
    I2C3_MCS_R = (0                // send stop if nonzero
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // stop
                     //  & ~I2C_MCS_START   // no start/restart
                     //  & ~I2C_MCS_RUN    // master disable
                        );   
                                          // return error bits if nonzero
    return (I2C3_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  }
  I2C3_MDR_R = data2&0xFF;         // prepare second byte
  I2C3_MCS_R = (0
                      // & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                      // & ~I2C_MCS_START   // no start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C3_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
	//*/
	/*
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (addr<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = data1&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                    //   & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // check error bits
  if((I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR)) != 0){
    I2C0_MCS_R = (0                // send stop if nonzero
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // stop
                     //  & ~I2C_MCS_START   // no start/restart
                     //  & ~I2C_MCS_RUN    // master disable
                        );   
                                          // return error bits if nonzero
    return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  }
  I2C0_MDR_R = data2&0xFF;         // prepare second byte
  I2C0_MCS_R = (0
                      // & ~I2C_MCS_ACK     // no data ack (no data on send)
                       | I2C_MCS_STOP     // generate stop
                      // & ~I2C_MCS_START   // no start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
                                          // return error bits
  return (I2C0_MCS_R&(I2C_MCS_DATACK|I2C_MCS_ADRACK|I2C_MCS_ERROR));
  */
}

uint32_t I2C_trans1_recv1(uint8_t addr, uint8_t dataOut, uint8_t *dataIn){
	///*
	while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C3_MSA_R = (addr<<1)&0xFE;    // MSA[7:1] is slave address
  I2C3_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C3_MDR_R = dataOut&0xFF;         // prepare first byte
  I2C3_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                    //   & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for transmission done

	while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C3_MSA_R = (addr<<1)&0xFE;    // MSA[7:1] is slave address
    I2C3_MSA_R |= 0x01;              // MSA[0] is 1 for receive
    I2C3_MCS_R = (0
                        // & ~I2C_MCS_ACK     // negative data ack (last byte)
                         | I2C_MCS_STOP     // generate stop
                         | I2C_MCS_START    // generate start/restart
                         | I2C_MCS_RUN);    // master enable
    while(I2C3_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
	*dataIn = (I2C3_MDR_R&0xFF);
	return I2C3_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR);
	//*/
	/*
	while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
  I2C0_MSA_R = (addr<<1)&0xFE;    // MSA[7:1] is slave address
  I2C0_MSA_R &= ~0x01;             // MSA[0] is 0 for send
  I2C0_MDR_R = dataOut&0xFF;         // prepare first byte
  I2C0_MCS_R = (0
                     //  & ~I2C_MCS_ACK     // no data ack (no data on send)
                    //   & ~I2C_MCS_STOP    // no stop
                       | I2C_MCS_START    // generate start/restart
                       | I2C_MCS_RUN);    // master enable
  while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done

	while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for I2C ready
    I2C0_MSA_R = (addr<<1)&0xFE;    // MSA[7:1] is slave address
    I2C0_MSA_R |= 0x01;              // MSA[0] is 1 for receive
    I2C0_MCS_R = (0
                        // & ~I2C_MCS_ACK     // negative data ack (last byte)
                         | I2C_MCS_STOP     // generate stop
                         | I2C_MCS_START    // generate start/restart
                         | I2C_MCS_RUN);    // master enable
    while(I2C0_MCS_R&I2C_MCS_BUSY){};// wait for transmission done
	*dataIn = (I2C0_MDR_R&0xFF);
	return I2C0_MCS_R&(I2C_MCS_ADRACK|I2C_MCS_ERROR);
	*/
}
