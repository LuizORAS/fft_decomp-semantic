/* SCUS_942.21 0x8001edec..0x8001ef2f. */
#include "psx/libcd.h"

/* Same as CdControl, then wait until the command completes (CD_sync); returns 1 when it completed. */
s32 CdControlB(u8 command, u8* parameter, u8* result) {
    if (psyq_cd_issue_with_retry(command, parameter, result, 0) != 0) {
        return 0;
    }
    return CD_sync(0, result) == CdlComplete;
}
