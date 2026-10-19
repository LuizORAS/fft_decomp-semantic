#include "psx/libetc.h"

/* Resume after stopIntr: hook the dispatcher into the BIOS's exception exit again, restore the saved
 * interrupt mask and DMA priorities and enable interrupts. Returns its state, or 0 when it is already running.
 * RestartCallback reaches it through the dispatch table. */
psyq_interrupt_state_t* restartIntr(void) {
    if (g_psyq_etc_interrupt_state.initialized)
        return 0;
    HookEntryInt(g_psyq_etc_interrupt_state.jump_buffer);
    g_psyq_etc_interrupt_state.initialized = 1;
    *g_psyq_etc_irq_mask = g_psyq_etc_interrupt_state.saved_mask;
    *g_psyq_etc_dma_dpcr = g_psyq_etc_interrupt_state.saved_dma_priority;
    ExitCriticalSection();
    return &g_psyq_etc_interrupt_state;
}
