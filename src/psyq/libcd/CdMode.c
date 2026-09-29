#include "psx/libcd.h"

/* LIBCD 0x8001e9dc-0x8001e9ec: first cached response location byte. */
s32 CdMode(void) {
    return g_psyq_cd_mode;
}
