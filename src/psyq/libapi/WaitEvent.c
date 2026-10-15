/* 0x80021f94: BIOS 0xb0:0x0a tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's WaitEvent (table B, 0x0a): wait until `event` is ready, set it busy again and return 1, or
 * return 0 for a disabled event (psx-spx). SpuClearReverbWorkArea waits on LIBSPU's DMA event after each
 * 1 KB transfer. */
s32 WaitEvent(s32 event) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_WAIT_EVENT);
}
