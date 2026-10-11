#include "psx/libpress.h"
#include "psx/types.h"

/* Spin until the MDEC input FIFO drains, reporting a timeout through the
 * library's diagnostic dump when the counter expires. */
s32 MDEC_in_sync(void) {
    volatile s32 count = 0x100000;

    while (*g_psyq_press_mdec_status & 0x20000000) {
        if (--count == -1) {
            timeout(g_psyq_press_in_sync_name);
            return -1;
        }
    }
    return 0;
}
