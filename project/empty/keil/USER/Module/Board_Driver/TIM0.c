#include "TIM0.h"

void tim0_init(void)
{
    NVIC_ClearPendingIRQ(TIMER_0_INST_INT_IRQN);
    NVIC_EnableIRQ(TIMER_0_INST_INT_IRQN);
    DL_Timer_enableInterrupt(TIMER_0_INST, DL_TIMER_INTERRUPT_LOAD_EVENT);
    DL_Timer_startCounter(TIMER_0_INST);
}
