#include "ti_msp_dl_config.h"
#include "systick.h"
#include "board_led.h"
#include "UART0_Debug.h"
#include "LCD.h"
#include "LCD_Init.h"
#include "board_key.h"
#include "TIM0.h"

int main(void)
{
    SYSCFG_DL_init();
    systick_timer_init(80);
    debug_uart_init(); // 串口中断初始化
    tim0_init();       // 定时器中断初始化
    LCD_Init();        // LCD初始化

    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    LCD_Printf(0, 0, WHITE, 12, "hello world");
    LCD_Printf(0, 20, WHITE, 12, "KeyNum:");

    while (1)
    {

        int i = 0;
        // 获取按键事件
        key_event_t event = key_get_event();

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
            for (i = 0; i <= 999; i++)
            {
                // 设置 LED 亮度
                DL_TimerG_setCaptureCompareValue(PWM_LED_INST, i, GPIO_PWM_LED_C0_IDX);
                systick_delay_ms(1); // 延迟以控制亮度变化速度
            }
            // 呼吸灯渐暗过程
            for (i = 999; i > 0; i--)
            {
                // 设置 LED 亮度
                DL_TimerG_setCaptureCompareValue(PWM_LED_INST, i, GPIO_PWM_LED_C0_IDX);
                systick_delay_ms(1); // 延迟以控制亮度变化速度
            }
        }
    }
}

void TIMER_0_INST_IRQHandler(void)
{
    static uint8_t count_1 = 0;
    // 如果产生了定时器中断
    switch (DL_TimerG_getPendingInterrupt(TIMER_0_INST))
    {
    case DL_TIMER_IIDX_LOAD: // 如果是0溢出中断

        count_1++;
        if(count_1 > 1){
            count_1 = 0;
        }
        key_tick();
        break;

    default: // 其他的定时器中断
        break;
    }
}