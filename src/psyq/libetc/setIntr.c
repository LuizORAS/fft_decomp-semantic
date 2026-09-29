/* LIBETC 8001e1e8-8001e33c; callback replacement and BIOS clear policy. */
#include "psx/libetc.h"

psyq_interrupt_callback_t setIntr(s32 slot, psyq_interrupt_callback_t callback) {
    /* Unpinning the table base changes argument copies and entry addressing. */
    register psyq_interrupt_callback_t* callbacks __asm__("$5") = g_psyq_etc_interrupt_state.callbacks;
    psyq_interrupt_state_t* state;
    psyq_interrupt_callback_t old;
    u32 saved_mask;
    s32 disabled;
    /* Keep state fields relative to the callback base instead of fresh addresses. */
    __asm__("" : "=r"(callbacks) : "0"(callbacks));
    state = (psyq_interrupt_state_t*)((u8*)callbacks - (u32) & ((psyq_interrupt_state_t*)0)->callbacks);
    old = callbacks[slot];
    if (callback != old && state->initialized) {
        /* Unpinning changes the halfword load and subsequent mask narrowing. */
        register u32 mask_word __asm__("$3") = *g_psyq_etc_irq_mask;
        *g_psyq_etc_irq_mask = 0;
        saved_mask = (u16)mask_word;
        if (callback) {
            u32 bit = 1U << slot;
            callbacks[slot] = callback;
            saved_mask |= bit;
            state->callback_mask |= bit;
        } else {
            /* This slot-pointer view retains clear-before-mask-reload ordering. */
            psyq_interrupt_callback_t* clear = &callbacks[slot];
            u32 keep = ~(1U << slot);
            *clear = 0;
            saved_mask &= keep;
            g_psyq_etc_callback_mask &= keep;
        }
        if (slot == PSYQ_IRQ_VBLANK) {
            disabled = callback == 0;
            ChangeClearPAD(disabled);
            ChangeClearRCnt(PSYQ_ETC_VBLANK_ROOT_COUNTER, disabled);
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
