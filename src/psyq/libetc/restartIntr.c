#include "psx/libetc.h"
psyq_interrupt_state_t* restartIntr(void) {
    psyq_interrupt_state_t* state = &g_psyq_etc_interrupt_state;
    if (state->initialized)
        return 0;
    HookEntryInt(state->jump_buffer);
    state->initialized = 1;
    *g_psyq_etc_irq_mask = g_psyq_etc_saved_irq_mask;
    *g_psyq_etc_dma_dpcr = g_psyq_etc_saved_dma_dpcr;
    ExitCriticalSection();
    return state;
}
