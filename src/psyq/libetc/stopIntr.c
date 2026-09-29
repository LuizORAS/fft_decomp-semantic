#include "psx/libetc.h"
psyq_interrupt_state_t* stopIntr(void) {
    psyq_interrupt_state_t* state = &g_psyq_etc_interrupt_state;
    if (!state->initialized)
        return 0;
    EnterCriticalSection();
    {
        volatile u16* mask = g_psyq_etc_irq_mask;
        g_psyq_etc_saved_irq_mask = *mask;
        {
            u32 priority = *g_psyq_etc_dma_dpcr;
            volatile u16* status = g_psyq_etc_irq_status;
            g_psyq_etc_saved_dma_dpcr = priority;
            *mask = 0;
            *status = *mask;
        }
    }
    *g_psyq_etc_dma_dpcr &= 0x77777777;
    ResetEntryInt();
    state->initialized = 0;
    return state;
}
