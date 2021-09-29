#ifndef EV3479_H
#define EV3479_H

#include <stdint.h>

#define X_LSB 0x0D
#define X_MSB 0x0E
#define Y_LSB 0x0F
#define Y_MSB 0x10
#define Z_LSB 0x11
#define Z_MSB 0x12

#define MC34X9_REG_DEV_STAT         (0x05)
#define MC34X9_REG_INTR_CTRL        (0x06)
#define MC34X9_REG_MODE             (0x07)
#define MC34X9_REG_SR               (0x08)
#define MC34X9_REG_MOTION_CTRL      (0x09)
#define MC34X9_REG_FIFO_STAT        (0x0A)
#define MC34X9_REG_FIFO_RD_P        (0x0B)
#define MC34X9_REG_FIFO_WR_P        (0x0C)
#define MC34X9_REG_XOUT_LSB         (0x0D)
#define MC34X9_REG_XOUT_MSB         (0x0E)
#define MC34X9_REG_YOUT_LSB         (0x0F)
#define MC34X9_REG_YOUT_MSB         (0x10)
#define MC34X9_REG_ZOUT_LSB         (0x11)
#define MC34X9_REG_ZOUT_MSB         (0x12)
#define MC34X9_REG_STATUS           (0x13)
#define MC34X9_REG_INTR_STAT        (0x14)
#define MC34X9_REG_PROD             (0x18)
#define MC34X9_REG_RANGE_C          (0x20)
#define MC34X9_REG_XOFFL            (0x21)
#define MC34X9_REG_XOFFH            (0x22)
#define MC34X9_REG_YOFFL            (0x23)
#define MC34X9_REG_YOFFH            (0x24)
#define MC34X9_REG_ZOFFL            (0x25)
#define MC34X9_REG_ZOFFH            (0x26)
#define MC34X9_REG_XGAIN            (0x27)
#define MC34X9_REG_YGAIN            (0x28)
#define MC34X9_REG_ZGAIN            (0x29)
#define MC34X9_REG_FIFO_CTRL        (0x2D)
#define MC34X9_REG_FIFO_TH          (0x2E)
#define MC34X9_REG_FIFO_INTR        (0x2F)
#define MC34X9_REG_FIFO_CTRL_SR2    (0x30)
#define MC34X9_REG_COMM_CTRL        (0x31)
#define MC34X9_REG_GPIO_CTRL        (0x33)
#define MC34X9_REG_TF_THRESH_LSB    (0x40)
#define MC34X9_REG_TF_THRESH_MSB    (0x41)
#define MC34X9_REG_TF_DB            (0x42)
#define MC34X9_REG_AM_THRESH_LSB    (0x43)
#define MC34X9_REG_AM_THRESH_MSB    (0x44)
#define MC34X9_REG_AM_DB            (0x45)
#define MC34X9_REG_SHK_THRESH_LSB   (0x46)
#define MC34X9_REG_SHK_THRESH_MSB   (0x47)
#define MC34X9_REG_PK_P2P_DUR_THRESH_LSB    (0x48)
#define MC34X9_REG_PK_P2P_DUR_THRESH_MSB    (0x49)
#define MC34X9_REG_TIMER_CTRL       (0x4A)

#define  MC34X9_MODE_SLEEP			0x00
#define  MC34X9_MODE_CWAKE 			0x01
#define  MC34X9_MODE_RESERVED 		0x02
#define  MC34X9_MODE_STANDBY 		0x03

#define  MC34X9_RANGE_2G			0x00
#define  MC34X9_RANGE_4G			0x01
#define  MC34X9_RANGE_8G			0x02
#define  MC34X9_RANGE_16G			0x03
#define  MC34X9_RANGE_12G			0x04

#define  MC34X9_SR_25Hz             0x10
#define  MC34X9_SR_50Hz             0x11
#define  MC34X9_SR_62_5Hz           0x12
#define  MC34X9_SR_100Hz            0x13
#define  MC34X9_SR_125Hz            0x14
#define  MC34X9_SR_250Hz            0x15
#define  MC34X9_SR_500Hz            0x16
#define  MC34X9_SR_DEFAULT_1000Hz   0x17

void EV3479_init(void);

void EV3479_measure(int16_t *x, int16_t *y, int16_t *z);

void EV3479_write(uint16_t addr, uint16_t data);

uint8_t EV3479_read(uint16_t addr);

#endif
