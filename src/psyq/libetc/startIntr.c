#include "psx/libetc.h"

/* Start LIBETC's interrupt handling, once: clear the interrupt status and mask, set the default DMA
 * priorities, clear its state, record a setjmp state on its own 4 KB stack and hook it into the BIOS's
 * exception exit (HookEntryInt), so trapIntr runs after each exception. Then install the VBlank and DMA
 * dispatchers (startIntrVSync, startIntrDMA) and their setters in the dispatch table, remove the BIOS's
 * CD-ROM handlers (_96_remove) and enable interrupts. Returns its state, or 0 when it already ran.
 * ResetCallback reaches it through the dispatch table. */
psyq_interrupt_state_t* startIntr(void) {
    if (g_psyq_etc_interrupt_state.initialized)
        return 0;
    *g_psyq_etc_irq_status = *g_psyq_etc_irq_mask = 0;
    *g_psyq_etc_dma_dpcr = PSYQ_DMA_DEFAULT_PRIORITY;
    psyq_etc_clear_interrupt_state_words(
        (u32*)&g_psyq_etc_interrupt_state, sizeof(g_psyq_etc_interrupt_state) / sizeof(u32));
    if (setjmp(g_psyq_etc_interrupt_state.jump_buffer))
        trapIntr();
    g_psyq_etc_interrupt_state.jump_buffer[1]
        = (u32)&g_psyq_etc_interrupt_state
              .stack[sizeof(g_psyq_etc_interrupt_state.stack) / sizeof(u32) - PSYQ_ETC_INTERRUPT_STACK_RESERVED_WORDS];
    HookEntryInt(g_psyq_etc_interrupt_state.jump_buffer);
    g_psyq_etc_interrupt_state.initialized = 1;
    g_psyq_etc_dispatch->vblank_callback = (void* (*)(s32, void*))startIntrVSync();
    {
        psyq_callback_setter_t setter = startIntrDMA();
        /* Retail loads the table pointer into a0, as if it were also passed to _96_remove; a plain store selects v1. */
        register psyq_interrupt_dispatch_t* dispatch __asm__("$4") = g_psyq_etc_dispatch;
        dispatch->dma_callback = setter;
    }
    _96_remove();
    ExitCriticalSection();
    return &g_psyq_etc_interrupt_state;
}
