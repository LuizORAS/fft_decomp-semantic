#include "psx/libetc.h"

/* Replace the interrupt controller's mask with `mask` and return the old one. LIBGPU's queue (_addque2,
 * _exeque, _reset, get_alarm) masks interrupts around its changes this way. */
s32 SetIntrMask(s32 mask) {
    s32 previous = *g_psyq_etc_irq_mask;
    *g_psyq_etc_irq_mask = mask;
    return previous;
}
