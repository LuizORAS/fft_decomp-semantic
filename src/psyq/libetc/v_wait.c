#include "psx/libetc.h"

/* Wait until the VBlank count reaches `vblank`, for at most `frames` << 15 polls. On a timeout, print a
 * message and stop the BIOS's handlers from acknowledging the VBlank interrupt themselves (ChangeClearPAD(0),
 * ChangeClearRCnt(3, 0)), so that trapIntrVSync sees it and the count moves again. VSync calls it. */
void v_wait(s32 vblank, s32 frames) {
    /* Retail declares the counter volatile: it is stored to and reloaded from the stack on every pass. */
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
