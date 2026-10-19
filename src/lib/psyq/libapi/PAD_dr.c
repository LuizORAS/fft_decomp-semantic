/* 0x80021ff4: BIOS 0xb0:0x16 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's PAD_dr (table B, 0x16): read both pads into the 32-bit value PAD_init2 named: pad 1 in the low half,
 * pad 2 in the high half, each byte-swapped, a 0 bit for each pressed button; a pad that is not a digital pad
 * reads as nothing pressed (psx-spx). PadRead calls it and reads that value. */
void PAD_dr(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_PAD_DR);
}
