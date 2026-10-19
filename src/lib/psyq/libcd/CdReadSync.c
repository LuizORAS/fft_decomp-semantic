/* SCUS_942.21 0x80020d44..0x80020e0f. */
#include "psx/libcd.h"
#include "psx/libetc.h"

/* Return how many sectors the CdRead in progress still has to read: 0 when it is done, -1 when it failed.
 * Mode 0 waits, mode 1 checks once. A read that failed, or had no sector for 60 VBlanks, restarts from its
 * first sector; after 1200 VBlanks it gives up. */
s32 CdReadSync(s32 mode, u8* result) {
    s32 remaining;
    while (1) {
        remaining = -1;
        if (VSync(-1) <= g_psyq_cd_read_state.start_clock + PSYQ_CD_READ_TIMEOUT_VBLANKS) {
            if (g_psyq_cd_read_state.remaining < 0
                || VSync(-1) > g_psyq_cd_read_state.sector_clock + PSYQ_CD_SECTOR_TIMEOUT_VBLANKS) {
                cd_read_retry(1);
                remaining = g_psyq_cd_read_state.sectors;
            } else {
                remaining = g_psyq_cd_read_state.remaining;
            }
        }
        if (mode || remaining <= 0)
            break;
    }
    CdReady(1, result);
    return remaining;
}
