/* SCUS_942.21 0x8001eb88..0x8001ecbf. */
#include "psx/libcd.h"

/* Send CD command `command` and wait for its acknowledgement, up to four tries. A command that takes a
 * position seeks there first (CdlSetloc), and an open lid's status is refreshed with a Nop before any other
 * command. Returns 1, or 0 when every try failed. */
s32 CdControl(u8 command, u8* parameter, u8* result) {
    return psyq_cd_issue_with_retry(command, parameter, result, 0) == 0;
}
