/* 0x80021f64: BIOS 0xb0:0x07 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's DeliverEvent (table B, 0x07): fire every enabled event of class `desc` and kind `spec`. LIBCD's
 * default callbacks (def_cbsync, def_cbready, def_cbread) and LIBSPU's DMA interrupt (_spu_FiDMA) report
 * through it. */
void DeliverEvent(u32 desc, u32 spec) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_DELIVER_EVENT);
}
