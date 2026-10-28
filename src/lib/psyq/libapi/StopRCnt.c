#include "psx/libapi.h"

/* Disable root counter `spec`'s interrupt in the interrupt controller's mask; returns 1 for any counter
 * (QUIRKS.md). No code refers to it. */
s32 StopRCnt(u32 spec) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    g_psyq_api_interrupt_controller->mask &= ~g_psyq_api_root_counter_irq_masks[counter];
    return 1;
}
