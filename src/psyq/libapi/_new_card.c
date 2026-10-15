/* 0x800287c8: BIOS 0xb0:0x50 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _new_card (table B, 0x50): make the BIOS ignore the card-changed flag on the next read or
 * write (psx-spx). _card_clear calls it before its dummy write. */
void _new_card(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_NEW_CARD);
}
