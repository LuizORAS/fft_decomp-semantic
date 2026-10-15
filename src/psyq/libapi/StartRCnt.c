#include "psx/libapi.h"

/* Enable root counter `spec`'s interrupt in the interrupt controller's mask; returns 1 for counters 0-2
 * (QUIRKS.md). SuzukiSPUInitialiser starts counter 2 with it. */
s32 StartRCnt(u32 spec) {
    s32 counter = spec & PSYQ_RCNT_SPEC_MASK;
    g_psyq_api_interrupt_controller->mask |= g_psyq_api_root_counter_irq_masks[counter];
    return counter < PSYQ_RCNT_HARDWARE_COUNT;
}
