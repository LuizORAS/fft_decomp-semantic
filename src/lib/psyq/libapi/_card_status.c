/* 0x80028770: BIOS 0xb0:0x5c tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _card_status (table B, 0x5c): the state of the last card command on `slot`: 1 ready; 2, 4 or 8
 * busy reading, writing or checking; 0x11 timeout (no card); 0x21 error (psx-spx). The card pollers test its
 * bit 0 while no event has come. */
s32 _card_status(s32 slot) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_CARD_STATUS);
}
