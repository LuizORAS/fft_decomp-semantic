/* SCUS_942.21 0x800202a8..0x800202f7. */
#include "psx/libc.h"
#include "psx/libcd.h"
#include "psx/libetc.h"

void CD_initintr(void) {
    g_psyq_cd_ready_callback = 0;
    g_psyq_cd_sync_callback = 0;
    g_psyq_cd_status_detail = 0;
    /* Retail resets all four status bytes with a word store. */
    *(u32*)&g_psyq_cd_status = 0;
    ResetCallback();
    InterruptCallback(2, callback);
}
