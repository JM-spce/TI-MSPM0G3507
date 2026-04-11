#include "board_key.h"

// 按键状态
typedef enum {
    KEY_STATE_IDLE = 0,       // 空闲状态
    KEY_STATE_DEBOUNCE,       // 消抖状态
    KEY_STATE_PRESSED,        // 按下状态
    KEY_STATE_RELEASE_WAIT,   // 等待释放（用于判断长按）
    KEY_STATE_WAIT_NEXT,      // 等待下一次点击（用于判断双击/三击）
} key_state_t;

static key_state_t key_state = KEY_STATE_IDLE;
static uint16_t key_timer = 0;
static uint8_t click_count = 0;
static key_event_t key_event = KEY_EVENT_NONE;

// 时间参数配置（单位：key_tick调用次数，假设每次调用间隔1-5ms）
#define DEBOUNCE_TIME        20     // 消抖时间 20*5ms = 100ms
#define SHORT_PRESS_TIME     200    // 短按最大时间 200*5ms = 1s
#define LONG_PRESS_TIME      600    // 长按触发时间 600*5ms = 3s
#define DOUBLE_CLICK_INTERVAL 300   // 双击间隔时间 300*5ms = 1.5s

void key_init(void)
{
    // GPIO已在SYSCFG_DL_GPIO_init()中初始化
    key_state = KEY_STATE_IDLE;
    key_timer = 0;
    click_count = 0;
    key_event = KEY_EVENT_NONE;
}

static uint8_t key_read_raw(void)
{
    return (DL_GPIO_readPins(KEY_PORT, KEY_B21_PIN) == 0) ? 1 : 0;
}

key_event_t key_get_event(void)
{
    key_event_t temp = key_event;
    key_event = KEY_EVENT_NONE;
    return temp;
}

const char* key_event_to_string(key_event_t event)
{
    switch(event)
    {
        case KEY_EVENT_SHORT_PRESS:  return "Short Press";
        case KEY_EVENT_LONG_PRESS:   return "Long Press";
        case KEY_EVENT_DOUBLE_CLICK: return "Double Click";
        case KEY_EVENT_TRIPLE_CLICK: return "Triple Click";
        default:                     return "None";
    }
}

void key_tick(void)
{
    static uint8_t raw_state = 0;
    
    switch(key_state)
    {
        case KEY_STATE_IDLE:
            // 检测按键按下
            if (key_read_raw())
            {
                key_state = KEY_STATE_DEBOUNCE;
                key_timer = 0;
            }
            break;
            
        case KEY_STATE_DEBOUNCE:
            key_timer++;
            if (key_read_raw())
            {
                if (key_timer >= DEBOUNCE_TIME)
                {
                    // 消抖完成，确认按下
                    key_state = KEY_STATE_PRESSED;
                    key_timer = 0;
                    click_count++;
                }
            }
            else
            {
                // 抖动，返回空闲
                key_state = KEY_STATE_IDLE;
                key_timer = 0;
            }
            break;
            
        case KEY_STATE_PRESSED:
            key_timer++;
            
            // 检测是否松开
            if (!key_read_raw())
            {
                // 按键松开
                if (key_timer < SHORT_PRESS_TIME)
                {
                    // 短按，进入等待状态看是否有后续点击
                    key_state = KEY_STATE_WAIT_NEXT;
                    key_timer = 0;
                }
                else
                {
                    // 已经超时，不应该到这里（应该在WAIT_RELEASE处理）
                    key_state = KEY_STATE_IDLE;
                    key_timer = 0;
                    click_count = 0;
                }
            }
            else if (key_timer >= LONG_PRESS_TIME)
            {
                // 长按触发
                key_event = KEY_EVENT_LONG_PRESS;
                key_state = KEY_STATE_RELEASE_WAIT;
            }
            break;
            
        case KEY_STATE_RELEASE_WAIT:
            // 长按已触发，等待按键释放
            if (!key_read_raw())
            {
                key_state = KEY_STATE_IDLE;
                key_timer = 0;
                click_count = 0;
            }
            break;
            
        case KEY_STATE_WAIT_NEXT:
            key_timer++;
            
            // 检测是否有新的按下
            if (key_read_raw())
            {
                if (key_timer < DOUBLE_CLICK_INTERVAL)
                {
                    // 在间隔时间内再次按下，继续计数
                    key_state = KEY_STATE_DEBOUNCE;
                    key_timer = 0;
                }
                else
                {
                    // 超过间隔时间，认为是新的独立按键
                    // 先输出之前的事件
                    if (click_count == 1)
                    {
                        key_event = KEY_EVENT_SHORT_PRESS;
                    }
                    else if (click_count == 2)
                    {
                        key_event = KEY_EVENT_DOUBLE_CLICK;
                    }
                    else if (click_count >= 3)
                    {
                        key_event = KEY_EVENT_TRIPLE_CLICK;
                    }
                    
                    // 重新开始计数
                    click_count = 0;
                    key_state = KEY_STATE_DEBOUNCE;
                    key_timer = 0;
                }
            }
            else if (key_timer >= DOUBLE_CLICK_INTERVAL)
            {
                // 超时没有新的点击，输出事件
                if (click_count == 1)
                {
                    key_event = KEY_EVENT_SHORT_PRESS;
                }
                else if (click_count == 2)
                {
                    key_event = KEY_EVENT_DOUBLE_CLICK;
                }
                else if (click_count >= 3)
                {
                    key_event = KEY_EVENT_TRIPLE_CLICK;
                }
                
                key_state = KEY_STATE_IDLE;
                key_timer = 0;
                click_count = 0;
            }
            break;
            
        default:
            key_state = KEY_STATE_IDLE;
            break;
    }
}
