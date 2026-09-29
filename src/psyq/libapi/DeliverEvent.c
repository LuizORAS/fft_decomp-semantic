/* 0x80021f64: BIOS 0xb0:0x07 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

void DeliverEvent(s32 desc, s32 spec) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_DELIVER_EVENT);
}
