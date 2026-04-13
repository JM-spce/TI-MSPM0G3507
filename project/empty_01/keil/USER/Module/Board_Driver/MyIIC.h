#ifndef MYIIC_H
#define MYIIC_H

#include "ti_msp_dl_config.h"
#include <stdint.h>

#define I2C_SCL_PIN IIC_IIC_SCL_PIN
#define I2C_SDA_PIN IIC_IIC_SDA_PIN

void MyI2C_W_SCL(uint8_t BitValue);
void MyI2C_W_SDA(uint8_t BitValue);
uint8_t MyI2C_R_SDA(void);
void MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

#endif