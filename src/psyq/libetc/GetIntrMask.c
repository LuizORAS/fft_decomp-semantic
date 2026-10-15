#include "psx/libetc.h"

/* Return the interrupt controller's mask. No code refers to it. */
s32 GetIntrMask(void) {
    return *g_psyq_etc_irq_mask;
}
