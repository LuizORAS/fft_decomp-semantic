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
s32 CdControlB(s32 command, const u8* parameter, u8* result) {
    /* Without this pin the retry constant is saved before the promoted command. */
    register s32 saved_command __asm__("$20") = command;
    u8 issued_command;
    s32 succeeded;
    /* Removing this tied use moves the retry constant above the promoted command. */
    __asm__("" : "=r"(saved_command) : "0"(saved_command));
    issued_command = saved_command;
    if (cd_issue_with_retry(issued_command, parameter, result, 0) == 0)
        succeeded = CD_sync(0, result) == CdlComplete;
    else
        succeeded = 0;
    return succeeded;
}
