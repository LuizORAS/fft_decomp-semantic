/* SCUS_942.21 0x8001edec..0x8001ef2f. */
#include "psx/libcd.h"

static __inline__ s32 cd_issue_with_retry(u8 command, const u8* parameter, u8* result, s32 asynchronous) {
    void* callback = g_psyq_cd_sync_callback;
    s32 attempts = 3;
    do {
        g_psyq_cd_sync_callback = 0;
        if (command != CdlNop && (g_psyq_cd_status & CdlStatShellOpen))
            CD_cw(CdlNop, 0, 0, 0);
        if (!parameter || !g_psyq_cd_command_uses_position[command] || CD_cw(CdlSetloc, parameter, result, 0) == 0) {
            g_psyq_cd_sync_callback = callback;
            if (CD_cw(command, parameter, result, asynchronous) == 0)
                return 0;
        }
    } while (--attempts != -1);
    g_psyq_cd_sync_callback = callback;
    return -1;
}

/* Same as CdControl, then wait until the command completes (CD_sync); returns 1 when it completed. */
s32 CdControlB(u8 command, const u8* parameter, u8* result) {
    if (cd_issue_with_retry(command, parameter, result, 0) != 0) {
        return 0;
    }
    return CD_sync(0, result) == CdlComplete;
}
