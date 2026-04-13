#ifndef __LCD_INIT_H
#define __LCD_INIT_H

#include "LCD.h"
#include "../ti_msp_dl_config.h"
#include <stdint.h>

#define USE_HORIZONTAL 1  //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏


#if USE_HORIZONTAL==0||USE_HORIZONTAL==1
#define LCD_W 128
#define LCD_H 160

#else
#define LCD_W 160
#define LCD_H 128
#endif

// 定义LCD引脚结构体类型
typedef struct {
    GPIO_Regs *port;
    uint32_t pin;
} LCD_Pin_t;


//-----------------LCD端口定义---------------- 

extern LCD_Pin_t LCD_CS_PIN;
extern LCD_Pin_t LCD_RES_PIN;
extern LCD_Pin_t LCD_DC_PIN;
extern LCD_Pin_t LCD_SCLK_PIN;
extern LCD_Pin_t LCD_MOSI_PIN;
extern LCD_Pin_t LCD_BLK_PIN;

#define LCD_SCLK_Clr() DL_GPIO_clearPins(LCD_SCLK_PIN.port, LCD_SCLK_PIN.pin)
#define LCD_SCLK_Set() DL_GPIO_setPins(LCD_SCLK_PIN.port, LCD_SCLK_PIN.pin)

#define LCD_MOSI_Clr() DL_GPIO_clearPins(LCD_MOSI_PIN.port, LCD_MOSI_PIN.pin)
#define LCD_MOSI_Set() DL_GPIO_setPins(LCD_MOSI_PIN.port, LCD_MOSI_PIN.pin)

#define LCD_RES_Clr()  DL_GPIO_clearPins(LCD_RES_PIN.port, LCD_RES_PIN.pin)
#define LCD_RES_Set()  DL_GPIO_setPins(LCD_RES_PIN.port, LCD_RES_PIN.pin)

#define LCD_DC_Clr()   DL_GPIO_clearPins(LCD_DC_PIN.port, LCD_DC_PIN.pin)
#define LCD_DC_Set()   DL_GPIO_setPins(LCD_DC_PIN.port, LCD_DC_PIN.pin)
 		     
#define LCD_CS_Clr()   DL_GPIO_clearPins(LCD_CS_PIN.port, LCD_CS_PIN.pin)
#define LCD_CS_Set()   DL_GPIO_setPins(LCD_CS_PIN.port, LCD_CS_PIN.pin)

#define LCD_BLK_Clr()  DL_GPIO_clearPins(LCD_BLK_PIN.port, LCD_BLK_PIN.pin)
#define LCD_BLK_Set()  DL_GPIO_setPins(LCD_BLK_PIN.port, LCD_BLK_PIN.pin)




void LCD_Writ_Bus(u8 dat);//模拟SPI时序
void LCD_WR_DATA8(u8 dat);//写入一个字节
void LCD_WR_DATA(u16 dat);//写入两个字节
void LCD_WR_REG(u8 dat);//写入一个指令
void LCD_Address_Set(u16 x1,u16 y1,u16 x2,u16 y2);//设置坐标函数
void LCD_Init(void);//LCD初始化
#endif



