#include "psx/libpress.h"
#include "psx/types.h"

/* Wait until the MDEC is no longer busy with a command (status bit 29, psx-spx); after 0x100000 polls,
 * report the timeout (timeout) and return -1. Returns 0 otherwise. */
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
