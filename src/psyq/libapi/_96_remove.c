/* 0x80021f44: BIOS 0xa0:0x72 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _96_remove (table A, 0x72): take the BIOS's own CD-ROM interrupt handlers out of its handler
 * chain. psx-spx notes that the BIOS's chain removal (SysDeqIntRP) is bugged, so this does not work.
 * startIntr calls it. */
void _96_remove(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_96_REMOVE);
}
