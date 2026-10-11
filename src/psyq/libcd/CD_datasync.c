/* SCUS_942.21 0x800204e4..0x8002064f. */
#include "psx/libc.h"
#include "psx/libcd.h"
#include "psx/libetc.h"

s32 CD_datasync(s32 mode) {
    s32 result;
    psyq_cd_set_timeout(g_psyq_cd_data_sync_timeout_name);
    while (1) {
        if (psyq_cd_check_timeout()) {
            result = -1;
            break;
        }
        if (!(*g_psyq_cd_dma_control_register & 0x01000000)) {
            result = 0;
            break;
        }
        if (mode) {
            result = 1;
            break;
        }
    }
    return result;
}
