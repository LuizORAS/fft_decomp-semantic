/* 0x80028760: BIOS 0xa0:0xad tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _card_auto (table A, 0xad): turn on (`mode` 1) or off (0) the BIOS's automatic formatting of a
 * card whose sector 0 does not read as a memory card (psx-spx). main_card_init_events turns it off. */
void _card_auto(s32 mode) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_CARD_AUTO);
}
