#include "LCD.h"
 
extern void func_int_to_str(char *str, int32_t num);
extern void func_uint_to_str(char *str, uint32_t num);
extern void func_double_to_str(char *str, double num, uint8_t pointnum);

#define LCD_write_8bit_data(data)                ( ST7735_WriteByte(data) )
#define LCD_write_16bit_data(data)				( ST7735_Write2Byte(data) )
 
static	uint8_t							LCD_show_h		= LCD_H;									// 屏幕高
static	uint8_t							LCD_show_w		= LCD_W;									// 屏幕宽
static 	uint16_t                   		LCD_pencolor     = LCD_DEFAULT_PENCOLOR;					// 画笔颜色(字体色)
static 	uint16_t                   		LCD_bgcolor      = LCD_DEFAULT_BGCOLOR; 					// 背景颜色
static 	LCD_font_size_enum    		LCD_display_font = LCD_DEFAULT_DISPLAY_FONT;      		// 显示字体类型
 
//-------------------------------------------------------------------------------------------------------------------
 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     发送一个字节（根据平台自行实现）
// 参数说明     TxData           	发送字节
// 返回参数     void
// 使用示例     ST7735_WriteByte(0xff);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void ST7735_WriteByte(uint8_t TxData)
{		
	DL_SPI_transmitData8(SPI_INST, TxData);
	while(DL_SPI_isBusy(SPI_INST));
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     发送两个字节（根据平台自行实现）
// 参数说明     TxData           	发送字节
// 返回参数     void
// 使用示例     ST7735_Write2Byte(RGB565_BLACK);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void ST7735_Write2Byte(uint16_t TxData)
{		
	DL_SPI_transmitData8(SPI_INST, (uint8_t)((TxData & 0xFF00) >> 8));
	while (DL_SPI_isBusy(SPI_INST));
	DL_SPI_transmitData8(SPI_INST, (uint8_t)(TxData & 0x00FF));
	while (DL_SPI_isBusy(SPI_INST));
}
 
//-------------------------------------------------------------------------------------------------------------------
 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     写命令
// 参数说明     dat             数据
// 返回参数     void
// 使用示例     LCD_write_index(0x2a);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void LCD_write_index (const uint8_t dat)
{
    LCD_DC(0);
    LCD_write_8bit_data(dat);
    LCD_DC(1);
}
 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示区域 内部调用
// 参数说明     x1              起始x轴坐标
// 参数说明     y1              起始y轴坐标
// 参数说明     x2              结束x轴坐标
// 参数说明     y2              结束y轴坐标
// 返回参数     void
// 使用示例     LCD_set_region(0, 0, LCD_width_max - 1, LCD_height_max - 1);
// 备注信息     内部调用
//-------------------------------------------------------------------------------------------------------------------
static void LCD_set_region (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if(LCD_show_h == LCD_H) {
		LCD_write_index(0x2a);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(x1 + 2);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(x2 + 2);
 
		LCD_write_index(0x2b);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(y1 + 1);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(y2 + 1);
	} else {
		LCD_write_index(0x2a);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(x1 + 1);
		LCD_write_8bit_data(0x0);
		LCD_write_8bit_data(x2 + 1);
 
		LCD_write_index(0x2b);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(y1 + 2);
		LCD_write_8bit_data(0x00);
		LCD_write_8bit_data(y2 + 2);
	}
	LCD_write_index(0x2c);
}
 
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 清屏函数
// 参数说明     void
// 返回参数     void
// 使用示例     LCD_clear();
// 备注信息     将屏幕清空成背景颜色
//-------------------------------------------------------------------------------------------------------------------
void LCD_clear (void)
{
    uint16_t i = LCD_show_w * LCD_show_h;
    LCD_CS(0);
    LCD_set_region(0, 0, LCD_show_w - 1, LCD_show_h - 1);
    for (; i != 0; i --)
    {
        LCD_write_16bit_data(LCD_bgcolor);
    }
    LCD_CS(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 清屏函数
// 参数说明     color           颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// 返回参数     void
// 使用示例     LCD_full(RGB565_YELLOW);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void LCD_full (const uint16_t color)
{
    uint16_t i = LCD_show_w * LCD_show_h;
    LCD_CS(0);
    LCD_set_region(0, 0, LCD_show_w - 1, LCD_show_h - 1);
    for (; i != 0; i --)
    {
        LCD_write_16bit_data(color);
    }
    LCD_CS(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     设置显示颜色
// 参数说明     pen             颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// 参数说明     bgcolor         颜色格式 RGB565 或者可以使用 zf_common_font.h 内常用颜色宏定义
// 返回参数     void
// 使用示例     LCD_set_color(RGB565_WHITE, RGB565_BLACK);
// 备注信息     字体颜色和背景颜色也可以随时自由设置 设置后生效
//-------------------------------------------------------------------------------------------------------------------
void LCD_set_color (uint16_t pen, const uint16_t bgcolor)
{
    LCD_pencolor = pen;
    LCD_bgcolor = bgcolor;
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 画点
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     dat             需要显示的颜色
// 返回参数     void
// 使用示例     LCD_draw_point(0, 0, RGB565_RED);            // 坐标 0,0 画一个红色的点
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void LCD_draw_point (uint16_t x, uint16_t y, const uint16_t color)
{
    LCD_CS(0);
    LCD_set_region(x, y, x, y);
    LCD_write_16bit_data(color);
    LCD_CS(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 画线
// 参数说明     x_start         坐标x方向的起点
// 参数说明     y_start         坐标y方向的起点
// 参数说明     x_end           坐标x方向的终点
// 参数说明     y_end           坐标y方向的终点
// 参数说明     dat             需要显示的颜色
// 返回参数     void
// 使用示例     LCD_draw_line(0, 0, 10, 10,RGB565_RED);      // 坐标 0,0 到 10,10 画一条红色的线
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void LCD_draw_line (uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t color)
{
    uint16_t x_dir = (x_start < x_end ? 1 : -1);
    uint16_t y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;
    do
    {
        if(x_start != x_end)
        {
            temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
            temp_b = (float)y_start - (float)x_start * temp_rate;
        }
        else
        {
            while(y_start != y_end)
            {
                LCD_draw_point(x_start, y_start, color);
                y_start += y_dir;
            }
            LCD_draw_point(x_start, y_start, color);
            break;
        }
        
        if(abs(y_start - y_end) > abs(x_start - x_end))
        {
            while(y_start != y_end)
            {
                LCD_draw_point(x_start, y_start, color);
                y_start += y_dir;
                x_start = (int16_t)(((float)y_start - temp_b) / temp_rate);
            }
            LCD_draw_point(x_start, y_start, color);
        }
        else
        {
            while(x_start != x_end)
            {
                LCD_draw_point(x_start, y_start, color);
                x_start += x_dir;
                y_start = (int16_t)((float)x_start * temp_rate + temp_b);
            }
            LCD_draw_point(x_start, y_start, color);
        }
    }while(0);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 显示字符
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     dat             需要显示的字符
// 返回参数     void
// 使用示例     LCD_show_char(0, 0, 'x');                    // 坐标 0,0 写一个字符 x
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void LCD_show_char (uint16_t x, uint16_t y, const char dat)
{
    uint8_t i = 0, j = 0;
    LCD_CS(0);
	switch(LCD_display_font)
	{
		case LCD_6X8_FONT:
		LCD_set_region(x, y, x + 5, y + 7);
		for(i = 0; 8 > i; i ++)
		{
			for(j = 0; 6 > j; j ++)
			{		
				// 减 32 因为是取模是从空格开始取得 空格在 ascii 中序号是 32
				uint8_t temp_top = ascii_font_6x8[dat - 32][j];
				if(temp_top & (0x01 << i))
				{
						LCD_write_16bit_data(LCD_pencolor);
				}
				else
				{
						LCD_write_16bit_data(LCD_bgcolor);
				}
				temp_top >>= 1;
			}
		}
		break;
		case LCD_8X16_FONT:
		LCD_set_region(x, y, x + 7, y + 15);
		
		for(i = 0; 8 > i; i ++)
		{
			for(j = 0; 8 > j; j ++)
			{
				uint8_t temp_top = ascii_font_8x16[dat - 32][j];
				if(temp_top & (0x01 << i))
				{
						LCD_write_16bit_data(LCD_pencolor);
				}
				else
				{
						LCD_write_16bit_data(LCD_bgcolor);
				}
				temp_top >>= 1;
			}
		}
		for(i = 0; 8 > i; i ++)
		{
			for(j = 8; 16 > j; j ++)
			{
				uint8_t temp_bottom = ascii_font_8x16[dat - 32][j];
				if(temp_bottom & (0x01 << i))
				{
						LCD_write_16bit_data(LCD_pencolor);
				}
				else
				{
						LCD_write_16bit_data(LCD_bgcolor);
				}
				temp_bottom >>= 1;
			}
		}
		break;
		default:
		break;
	}
    LCD_CS(1);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 显示字符串
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     dat             需要显示的字符串
// 返回参数     void
// 使用示例     LCD_show_string(0, 0, "seekfree");
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void LCD_show_string (uint16_t x, uint16_t y, const char dat[])
{   
    uint16_t j = 0;
    while('\0' != dat[j])
    {
        switch(LCD_display_font)
        {
            case LCD_6X8_FONT:   LCD_show_char(x + 6 * j, y, dat[j]); break;
            case LCD_8X16_FONT:  LCD_show_char(x + 8 * j, y, dat[j]); break;
            case LCD_16X16_FONT: break;                                      // 暂不支持
        }
        j ++;
    }
}
//-------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 显示32位有符号 (去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     dat             需要显示的变量 数据类型 int32
// 参数说明     num             需要显示的位数 最高10位  不包含正负号
// 返回参数     void
// 使用示例     LCD_show_int(0, 0, x, 3);                    // x 可以为 int32 int16 int8 类型
// 备注信息     负数会显示一个 ‘-’号
//-------------------------------------------------------------------------------------------------------------------
void LCD_show_int (uint16_t x, uint16_t y, const int32_t dat, uint8_t num)
{
    char data_buffer[12];
    int32_t dat_temp = dat;
    int32_t offset = 1;

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    if(num > 0 && num < 10)
    {
        for(uint8_t i = 0; i < num; i++)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    
    // 使用 sprintf 替代 func_int_to_str
    sprintf(data_buffer, "%ld", (long)dat_temp);
    
    LCD_show_string(x, y, data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 显示32位无符号 (去除整数部分无效的0)
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     dat             需要显示的变量 数据类型 uint32
// 参数说明     num             需要显示的位数 最高10位
// 返回参数     void
// 使用示例     LCD_show_uint(0, 0, x, 3);                   // x 可以为 uint32 uint16 uint8 类型
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void LCD_show_uint (uint16_t x, uint16_t y, const uint32_t dat, uint8_t num)
{
    char data_buffer[12];
    uint32_t dat_temp = dat;
    uint32_t offset = 1;

    // 用来计算余数显示 123 显示 2 位则应该显示 23
    if(num > 0 && num < 10)
    {
        for(uint8_t i = 0; i < num; i++)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    
    // 使用 sprintf 替代 func_uint_to_str
    sprintf(data_buffer, "%lu", (unsigned long)dat_temp);
    
    LCD_show_string(x, y, data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 显示浮点数
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     dat             需要显示的变量 数据类型 double
// 参数说明     num             整数部分显示的位数
// 参数说明     pointnum        小数部分显示的位数
// 返回参数     void
// 使用示例     LCD_show_float(0, 0, 3.1415, 1, 3);          // 显示 3.141
// 备注信息     
//-------------------------------------------------------------------------------------------------------------------
void LCD_show_float (uint16_t x, uint16_t y, const double dat, uint8_t num, uint8_t pointnum)
{
    char data_buffer[20];
    double dat_temp = dat;
    
    // 限制小数位数防止缓冲区溢出
    if(pointnum > 6) pointnum = 6;
    
    // 使用 sprintf 替代 func_double_to_str
    // 格式字符串例如: "%.3f"
    char format[10];
    sprintf(format, "%%.%df", pointnum);
    sprintf(data_buffer, format, dat_temp);
    
    LCD_show_string(x, y, data_buffer);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 格式化输出（类似printf）
// 参数说明     x               坐标x方向的起点 参数范围 [0, LCD_width_max-1]
// 参数说明     y               坐标y方向的起点 参数范围 [0, LCD_height_max-1]
// 参数说明     format          格式化字符串
// 返回参数     void
// 使用示例     LCDprint(0, 0, "Value: %d, Float: %.2f", 123, 3.14);
// 备注信息     支持 %d, %u, %f, %s, %c, %x, %o 等常用格式
//-------------------------------------------------------------------------------------------------------------------
void LCDprint(uint16_t x, uint16_t y, const char *format, ...)
{
    static char buffer[64];
    va_list args;
    
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    LCD_show_string(x, y, buffer);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     LCD 初始化
// 返回参数     void
// 返回参数     void
// 使用示例     LCD_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void LCD_init (void)
{
    LCD_set_color(LCD_pencolor, LCD_bgcolor);
 
    LCD_RES(0);
    Delay_ms(10);
 
    LCD_RES(1);
    Delay_ms(120);
    LCD_CS(0);
 
    LCD_write_index(0x11);
    Delay_ms(120);
 
    LCD_write_index(0xB1);
    LCD_write_8bit_data(0x01);
    LCD_write_8bit_data(0x2C);
    LCD_write_8bit_data(0x2D);
 
    LCD_write_index(0xB2);
    LCD_write_8bit_data(0x01);
    LCD_write_8bit_data(0x2C);
    LCD_write_8bit_data(0x2D);
 
    LCD_write_index(0xB3);
    LCD_write_8bit_data(0x01);
    LCD_write_8bit_data(0x2C);
    LCD_write_8bit_data(0x2D);
    LCD_write_8bit_data(0x01);
    LCD_write_8bit_data(0x2C);
    LCD_write_8bit_data(0x2D);
 
    LCD_write_index(0xB4);
    LCD_write_8bit_data(0x07);
 
    LCD_write_index(0xC0);
    LCD_write_8bit_data(0xA2);
    LCD_write_8bit_data(0x02);
    LCD_write_8bit_data(0x84);
    LCD_write_index(0xC1);
    LCD_write_8bit_data(0xC5);
 
    LCD_write_index(0xC2);
    LCD_write_8bit_data(0x0A);
    LCD_write_8bit_data(0x00);
 
    LCD_write_index(0xC3);
    LCD_write_8bit_data(0x8A);
    LCD_write_8bit_data(0x2A);
    LCD_write_index(0xC4);
    LCD_write_8bit_data(0x8A);
    LCD_write_8bit_data(0xEE);
 
    LCD_write_index(0xC5);
    LCD_write_8bit_data(0x0E);
 
    LCD_write_index(0x36);
    switch(LCD_DEFAULT_DISPLAY_DIR)                                                  // y x v
    {
        case LCD_PORTAIT:        
			LCD_write_8bit_data(1<<7 | 1<<6 | 0<<5);  
			break;
        case LCD_PORTAIT_180:    
			LCD_write_8bit_data(0<<7 | 0<<6 | 0<<5);  
			break;
        case LCD_CROSSWISE:      
			LCD_write_8bit_data(1<<7 | 0<<6 | 1<<5);
			LCD_show_h = LCD_W;
			LCD_show_w = LCD_H;
			break;
        case LCD_CROSSWISE_180:  
			LCD_write_8bit_data(0<<7 | 1<<6 | 1<<5);
			LCD_show_h = LCD_W;
			LCD_show_w = LCD_H;
			break;
    }
 
    LCD_write_index(0xe0);
    LCD_write_8bit_data(0x0f);
    LCD_write_8bit_data(0x1a);
    LCD_write_8bit_data(0x0f);
    LCD_write_8bit_data(0x18);
    LCD_write_8bit_data(0x2f);
    LCD_write_8bit_data(0x28);
    LCD_write_8bit_data(0x20);
    LCD_write_8bit_data(0x22);
    LCD_write_8bit_data(0x1f);
    LCD_write_8bit_data(0x1b);
    LCD_write_8bit_data(0x23);
    LCD_write_8bit_data(0x37);
    LCD_write_8bit_data(0x00);
    LCD_write_8bit_data(0x07);
    LCD_write_8bit_data(0x02);
    LCD_write_8bit_data(0x10);
 
    LCD_write_index(0xe1);
    LCD_write_8bit_data(0x0f);
    LCD_write_8bit_data(0x1b);
    LCD_write_8bit_data(0x0f);
    LCD_write_8bit_data(0x17);
    LCD_write_8bit_data(0x33);
    LCD_write_8bit_data(0x2c);
    LCD_write_8bit_data(0x29);
    LCD_write_8bit_data(0x2e);
    LCD_write_8bit_data(0x30);
    LCD_write_8bit_data(0x30);
    LCD_write_8bit_data(0x39);
    LCD_write_8bit_data(0x3f);
    LCD_write_8bit_data(0x00);
    LCD_write_8bit_data(0x07);
    LCD_write_8bit_data(0x03);
    LCD_write_8bit_data(0x10);
 
    LCD_write_index(0x2a);
    LCD_write_8bit_data(0x00);
    LCD_write_8bit_data(0x00 + 2);
    LCD_write_8bit_data(0x00);
    LCD_write_8bit_data(0x80 + 2);
 
    LCD_write_index(0x2b);
    LCD_write_8bit_data(0x00);
    LCD_write_8bit_data(0x00 + 3);
    LCD_write_8bit_data(0x00);
    LCD_write_8bit_data(0x80 + 3);
 
    LCD_write_index(0xF0);
    LCD_write_8bit_data(0x01);
    LCD_write_index(0xF6);
    LCD_write_8bit_data(0x00);
 
    LCD_write_index(0x3A);
    LCD_write_8bit_data(0x05);
    LCD_write_index(0x29);
    LCD_CS(1);
 
    
    LCD_full(RGB565_BLACK);
    LCD_clear();
}