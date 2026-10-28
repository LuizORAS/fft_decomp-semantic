#include "psx/libetc.h"

/* Stop LIBETC's interrupt handling: disable interrupts, save the interrupt mask and DMA priorities, clear
 * the interrupt status and mask, disable every DMA channel and put back the BIOS's default exception exit
 * (ResetEntryInt). Interrupts stay disabled. Returns its state, or 0 when it was not running. StopCallback
 * reaches it through the dispatch table. */
psyq_interrupt_state_t* stopIntr(void) {
    if (!g_psyq_etc_interrupt_state.initialized)
        return 0;
    EnterCriticalSection();
    g_psyq_etc_interrupt_state.saved_mask = *g_psyq_etc_irq_mask;
    g_psyq_etc_interrupt_state.saved_dma_priority = *g_psyq_etc_dma_dpcr;
    *g_psyq_etc_irq_status = *g_psyq_etc_irq_mask = 0;
    *g_psyq_etc_dma_dpcr &= 0x77777777;
    ResetEntryInt();
    g_psyq_etc_interrupt_state.initialized = 0;
    return &g_psyq_etc_interrupt_state;
}
