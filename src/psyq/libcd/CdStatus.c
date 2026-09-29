#include "psx/libcd.h"

/* LIBCD 0x8001e9cc-0x8001e9dc: cached controller status byte. */
s32 CdStatus(void) {
    return g_psyq_cd_status;
}
