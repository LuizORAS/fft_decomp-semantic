/* 0x80021fe4: BIOS 0xb0:0x15 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's PAD_init2 (table B, 0x15): start the BIOS's pad handler, which then stores both pads' buttons
 * at `buttons` from its interrupt; `type` must be 0x20000000 or 0x20000001 (psx-spx). PadInit calls it with
 * 0x20000001. */
s32 PAD_init2(s32 type, u32* buttons) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_PAD_INIT2);
}
