/* LIBETC 8001e000-8001e1e8; IRQ acknowledgement and callback dispatch. */
#include "psx/libetc.h"

/* LIBETC's interrupt dispatcher, entered after each BIOS exception through the hook startIntr installs:
 * until no enabled interrupt with a callback is pending, acknowledge each one and call its callback, with
 * CheckCallback reporting 1. Pending interrupts that nothing handles are counted; after more than 2048 calls in
 * a row it prints them and clears the status. It ends with ReturnFromException; before startIntr it only
 * prints the status. */
void trapIntr(void) {
    s32 slot;
    u16 pending;
    if (!g_psyq_etc_interrupt_state.initialized) {
        printf(g_psyq_etc_uninitialized_irq_format, *g_psyq_etc_irq_status);
        ReturnFromException();
    }
    g_psyq_etc_interrupt_state.in_callback = 1;
    pending = (g_psyq_etc_interrupt_state.callback_mask & *g_psyq_etc_irq_status) & *g_psyq_etc_irq_mask;
    while (pending) {
        for (slot = 0; pending && slot < PSYQ_IRQ_SOURCES; slot++, pending >>= 1) {
            if (pending & 1) {
                *g_psyq_etc_irq_status = ~(1 << slot);
                if (g_psyq_etc_interrupt_state.callbacks[slot])
                    g_psyq_etc_interrupt_state.callbacks[slot]();
            }
        }
        pending = (g_psyq_etc_interrupt_state.callback_mask & *g_psyq_etc_irq_status) & *g_psyq_etc_irq_mask;
    }
    if (*g_psyq_etc_irq_status & *g_psyq_etc_irq_mask) {
        if (g_psyq_etc_unhandled_irq_count++ > PSYQ_ETC_UNHANDLED_IRQ_LIMIT) {
            printf(g_psyq_etc_unhandled_irq_format, *g_psyq_etc_irq_status, *g_psyq_etc_irq_mask);
            g_psyq_etc_unhandled_irq_count = 0;
            *g_psyq_etc_irq_status = 0;
        }
    } else {
        g_psyq_etc_unhandled_irq_count = 0;
    }
    g_psyq_etc_interrupt_state.in_callback = 0;
    ReturnFromException();
}
