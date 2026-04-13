#include "ti_msp_dl_config.h"
#include "systick.h"
#include "board_led.h"
#include "UART0_Debug.h"
#include "LCD.h"
#include "LCD_Init.h"
#include "board_key.h"
#include "TIM0.h"
#include "mpu6050.h"

int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
int16_t i = 0;

int main(void)
{
    SYSCFG_DL_init();
    systick_timer_init(80);
    debug_uart_init(); // 串口中断初始化
    tim0_init();       // 定时器中断初始化
    LCD_Init();        // LCD初始化

    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    MPU6050_Init(); // MPU6050初始化

    while (1)
    {
        // 获取按键事件
        key_event_t event = key_get_event();
        // MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);
        if (event != KEY_EVENT_NONE)
        {
            switch (event)
            {
            case KEY_EVENT_SHORT_PRESS:
                // 短按处理（单击后1.5秒内无第二次点击）
                board_led_toggle();
                printf("1\r\n");
                LCD_Printf(0, 16, WHITE, 12, "Key Event: %d", 1);
                break;

            case KEY_EVENT_LONG_PRESS:
                // 长按处理（按住超过3秒）
                printf("4\r\n");
                LCD_Printf(0, 16, WHITE, 12, "Key Event: %d", 4);
                break;

            case KEY_EVENT_DOUBLE_CLICK:
                // 双击处理（1.5秒内连续点击2次）
                printf("2\r\n");
                LCD_Printf(0, 16, WHITE, 12, "Key Event: %d", 2);
                break;

            case KEY_EVENT_TRIPLE_CLICK:
                // 三击处理（1.5秒内连续点击3次或以上）
                printf("3\r\n");
                LCD_Printf(0, 16, WHITE, 12, "Key Event: %d", 3);
                break;

            default:
                break;
            }
        }

        LCD_ShowString(0 * 6, 0 * 12, (const u8 *)"AccX:", WHITE, BLACK, 12, 0);
        LCD_ShowIntNum(6 * 6, 0 * 12, AccX, 6, WHITE, BLACK, 12);
        LCD_ShowString(0 * 6, 1 * 12, (const u8 *)"AccY:", WHITE, BLACK, 12, 0);
        LCD_ShowIntNum(6 * 6, 1 * 12, AccY, 6, WHITE, BLACK, 12);
        LCD_ShowString(0, 32, (const u8*)"AccZ:", WHITE, BLACK, 12, 0);
        LCD_ShowIntNum(40, 32, AccZ, 6, WHITE, BLACK, 12);
        LCD_ShowString(0, 48, (const u8*)"GyroX:", WHITE, BLACK, 12, 0);
        LCD_ShowIntNum(48, 48, GyroX, 6, WHITE, BLACK, 12);
        LCD_ShowString(80, 48, (const u8*)"GyroY:", WHITE, BLACK, 12, 0);
        LCD_ShowIntNum(128, 48, GyroY, 6, WHITE, BLACK, 12);
        LCD_ShowString(0, 64, (const u8*)"i:", WHITE, BLACK, 12, 0);
        LCD_ShowIntNum(16, 64, i, 6, WHITE, BLACK, 12);
        i++;
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    static uint8_t breathing_state = 0; // 0:渐亮, 1:渐暗
    static uint16_t pwm_value = 0;

    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST))
    {
    case DL_TIMER_IIDX_LOAD:
        if (breathing_state == 0)
        {
            pwm_value++;
            if (pwm_value >= 999)
            {
                breathing_state = 1;
            }
        }
        else
        {
            pwm_value--;
            if (pwm_value == 0)
            {
                breathing_state = 0;
            }
        }
        DL_TimerG_setCaptureCompareValue(PWM_LED_INST, pwm_value, GPIO_PWM_LED_C0_IDX);

        key_tick();
        break;

    default:
        break;
    }
}