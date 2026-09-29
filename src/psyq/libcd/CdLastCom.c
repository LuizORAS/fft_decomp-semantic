#include "psx/libcd.h"

/* LIBCD 0x8001e9ec-0x8001e9fc: second cached response location byte. */
s32 CdLastCom(void) {
    return g_psyq_cd_last_command;
}
