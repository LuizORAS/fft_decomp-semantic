/* LIBETC 8001e000-8001e1e8; IRQ acknowledgement and callback dispatch. */
#include "psx/libetc.h"

void trapIntr(void) {
    psyq_interrupt_state_t* state = &g_psyq_etc_interrupt_state;
    u16 pending;
    s32 slot;
    u32 one;
    psyq_interrupt_callback_t* callbacks;
    psyq_interrupt_callback_t* callback;
    volatile u16* status;
    volatile u16* mask;
    if (!state->initialized) {
        printf(g_psyq_etc_uninitialized_irq_format, *g_psyq_etc_irq_status);
        ReturnFromException();
    }
    g_psyq_etc_in_callback = 1;
    pending = (g_psyq_etc_callback_mask & *g_psyq_etc_irq_status) & *g_psyq_etc_irq_mask;
    if (pending) {
        one = 1;
        callbacks = state->callbacks;
        do {
            slot = 0;
            if (pending) {
                callback = callbacks;
                while (pending) {
                    if (slot >= PSYQ_IRQ_SOURCES)
                        break;
                    if (pending & 1) {
                        *g_psyq_etc_irq_status = ~(one << slot);
                        if (*callback)
                            (*callback)();
                    }
                    callback++;
                    pending >>= 1;
                    slot++;
                }
            }
            pending = (g_psyq_etc_callback_mask & *g_psyq_etc_irq_status) & *g_psyq_etc_irq_mask;
        } while (pending);
    }
    status = g_psyq_etc_irq_status;
    mask = g_psyq_etc_irq_mask;
    if (*status & *mask) {
        if (g_psyq_etc_unhandled_irq_count++ > PSYQ_ETC_UNHANDLED_IRQ_LIMIT) {
            printf(g_psyq_etc_unhandled_irq_format, *status, *mask);
            g_psyq_etc_unhandled_irq_count = 0;
            *g_psyq_etc_irq_status = 0;
        }
    } else {
        g_psyq_etc_unhandled_irq_count = 0;
    }
    g_psyq_etc_in_callback = 0;
    ReturnFromException();
}
