#include "MyIIC.h"
#include "systick.h"

#define I2C_DELAY_US 5

static void i2c_delay_us(uint32_t us)
{
    volatile uint32_t i;
    for (i = 0; i < us * 10; i++)
    {
        __NOP();
    }
}

void MyI2C_W_SCL(uint8_t BitValue)
{
    if (BitValue)
    {
        DL_GPIO_setPins(IIC_PORT, I2C_SCL_PIN);
    }
    else
    {
        DL_GPIO_clearPins(IIC_PORT, I2C_SCL_PIN);
    }
    i2c_delay_us(I2C_DELAY_US);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
    if (BitValue)
    {
        DL_GPIO_setPins(IIC_PORT, I2C_SDA_PIN);
    }
    else
    {
        DL_GPIO_clearPins(IIC_PORT, I2C_SDA_PIN);
    }
    i2c_delay_us(I2C_DELAY_US);
}

uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = (DL_GPIO_readPins(IIC_PORT, I2C_SDA_PIN)) ? 1 : 0;
    i2c_delay_us(I2C_DELAY_US);
    return BitValue;
}

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(0);
}

void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);
    MyI2C_W_SCL(1);
    MyI2C_W_SDA(1);
}

void MyI2C_SendByte(uint8_t Byte)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SDA(!!(Byte & (0x80 >> i)));
        MyI2C_W_SCL(1);
        MyI2C_W_SCL(0);
    }
}

uint8_t MyI2C_ReceiveByte(void)
{
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);
        if (MyI2C_R_SDA())
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);
    }
    return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
    MyI2C_W_SDA(AckBit);
    MyI2C_W_SCL(1);
    MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);
    MyI2C_W_SCL(1);
    AckBit = MyI2C_R_SDA();
    MyI2C_W_SCL(0);
    return AckBit;
}