#include "ti_msp_dl_config.h"
#include "systick.h"
#include "board_led.h"
#include "UART0_Debug.h"
#include "LCD.h"
#include "board_key.h"
#include "TIM0.h"
#include "mpu6050.h"
#include "MahonyAHRS.h"

float gyroOffsetX = 0, gyroOffsetY = 0, gyroOffsetZ = 0;
int16_t AccX, AccY, AccZ, GyroX, GyroY, GyroZ;
float pitch, roll, yaw;
int16_t i = 0;

int main(void)
{
    SYSCFG_DL_init();
    systick_timer_init(80);
    debug_uart_init(); // 串口中断初始化
    tim0_init();       // 定时器中断初始化
    LCD_init();        // LCD初始化

    MPU6050_Init(); // MPU6050初始化
    LCD_full(RGB565_BLACK);

    while (1)
    {
        MPU6050_GetData(&AccX, &AccY, &AccZ, &GyroX, &GyroY, &GyroZ);

        // 2. 陀螺仪减去静态零偏（校准值）
        float gx = (GyroX - gyroOffsetX) / 131.0f;
        float gy = (GyroY - gyroOffsetY) / 131.0f;
        float gz = (GyroZ - gyroOffsetZ) / 131.0f;

        // 3. 加速度计单位转换
        float ax = AccX / 16384.0f;
        float ay = AccY / 16384.0f;
        float az = AccZ / 16384.0f;

        MahonyAHRSupdate(gx, gy, gz, ax, ay, az, 0, 0, 0);
        Mahony_GetEuler(&pitch, &roll, &yaw);

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
                break;

            case KEY_EVENT_LONG_PRESS:
                // 长按处理（按住超过3秒）
                printf("4\r\n");
                break;

            case KEY_EVENT_DOUBLE_CLICK:
                // 双击处理（1.5秒内连续点击2次）
                printf("2\r\n");
                break;

            case KEY_EVENT_TRIPLE_CLICK:
                // 三击处理（1.5秒内连续点击3次或以上）
                printf("3\r\n");
                break;

            default:
                break;
            }
        }

        LCDprint(0 * 6, 0 * 8, "AccX:%d", AccX);
        LCDprint(11 * 6, 0 * 8, "AccY:%d", AccY);
        LCDprint(0 * 6, 1 * 8, "AccZ:%d", AccZ);
        LCDprint(11 * 6, 1 * 8, "GyroX:%d", GyroX);
        LCDprint(0 * 6, 2 * 8, "GyroY:%d", GyroY);
        LCDprint(11 * 6, 2 * 8, "GyroZ:%d", GyroZ);
        LCDprint(0 * 6, 3 * 8, "pitch:%.2f", pitch);
        LCDprint(11 * 6, 3 * 8, "roll:%.2f", roll);
        LCDprint(0 * 6, 4 * 8, "yaw: %.2f", yaw);
        LCDprint(12 * 6, 5 * 8, "i: %d", i);
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