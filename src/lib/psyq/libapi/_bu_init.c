/* 0x80021f34: BIOS 0xa0:0x70 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _bu_init (table A, 0x70): set up the BIOS's memory card file access (the "bu" devices) with
 * automatic formatting off (psx-spx). main_card_init_events calls it after InitCARD and StartCARD. */
void _bu_init(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_BU_INIT);
}
