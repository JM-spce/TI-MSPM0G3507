#ifndef BOARD_KEY_H
#define BOARD_KEY_H

#include "ti_msp_dl_config.h"

typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_SHORT_PRESS,    // 短按
    KEY_EVENT_LONG_PRESS,     // 长按
    KEY_EVENT_DOUBLE_CLICK,   // 双击
    KEY_EVENT_TRIPLE_CLICK,   // 三击
} key_event_t;

void key_init(void);
key_event_t key_get_event(void);
const char* key_event_to_string(key_event_t event);
void key_tick(void);

#endif