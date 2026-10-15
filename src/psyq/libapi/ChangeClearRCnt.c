/* 0x80022104: BIOS 0xc0:0x0a tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's ChangeClearRCnt (table C, 0x0a): choose whether the BIOS's handler for root counter `counter`
 * (3 for the VBlank) acknowledges its interrupt and returns from the exception at once (`mode` 1) or leaves it
 * (0); returns the previous choice (psx-spx). setIntr sets 0 when a LIBETC callback takes the interrupt and 1
 * when the callback is removed; v_wait sets 0 for the VBlank after a timeout. */
s32 ChangeClearRCnt(s32 counter, s32 mode) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_C, PSYQ_BIOS_C_CHANGE_CLEAR_RCNT);
}
