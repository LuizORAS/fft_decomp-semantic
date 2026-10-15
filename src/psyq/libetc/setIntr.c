/* LIBETC 8001e1e8-8001e33c; callback replacement and BIOS clear policy. */
#include "psx/libetc.h"

/* Set the callback of interrupt `slot` and return the previous one. With the interrupt mask cleared, it
 * stores the callback, enables or disables the interrupt in the mask, and for the VBlank and root counters
 * 0-2 tells the BIOS's handlers to leave the interrupt to the callback, or to acknowledge it again when the
 * callback is removed (ChangeClearPAD, ChangeClearRCnt). It changes nothing before startIntr has run.
 * InterruptCallback reaches it through the dispatch table. */
psyq_interrupt_callback_t setIntr(s32 slot, psyq_interrupt_callback_t callback) {
    psyq_interrupt_callback_t old;
    s32 saved_mask;
    old = g_psyq_etc_interrupt_state.callbacks[slot];
    if (callback != old && g_psyq_etc_interrupt_state.initialized) {
        saved_mask = *g_psyq_etc_irq_mask;
        *g_psyq_etc_irq_mask = 0;
        if (callback) {
            g_psyq_etc_interrupt_state.callbacks[slot] = callback;
            saved_mask |= 1 << slot;
            g_psyq_etc_interrupt_state.callback_mask |= 1 << slot;
        } else {
            g_psyq_etc_interrupt_state.callbacks[slot] = 0;
            saved_mask &= ~(1 << slot);
            g_psyq_etc_interrupt_state.callback_mask &= ~(1 << slot);
        }
        if (slot == PSYQ_IRQ_VBLANK) {
            ChangeClearPAD(callback == 0);
            ChangeClearRCnt(PSYQ_ETC_VBLANK_ROOT_COUNTER, callback == 0);
        }
        if (slot == PSYQ_IRQ_TIMER0)
            ChangeClearRCnt(0, callback == 0);
        if (slot == PSYQ_IRQ_TIMER1)
            ChangeClearRCnt(1, callback == 0);
        if (slot == PSYQ_IRQ_TIMER2)
            ChangeClearRCnt(2, callback == 0);
        *g_psyq_etc_irq_mask = saved_mask;
    }
    return old;
}
