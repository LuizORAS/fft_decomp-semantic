#include "psx/libetc.h"
void v_wait(s32 vblank, s32 frames) {
    /* Volatile keeps the original stack timer stores and reloads. */
    volatile s32 timeout = frames << PSYQ_ETC_VSYNC_TIMEOUT_SHIFT;
    while (g_psyq_etc_vblank_count < vblank) {
        if (--timeout == -1) {
            puts(g_psyq_etc_vsync_timeout_message);
            ChangeClearPAD(0);
            ChangeClearRCnt(PSYQ_ETC_VBLANK_ROOT_COUNTER, 0);
            break;
        }
    }
}
