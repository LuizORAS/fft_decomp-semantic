/* 0x80021fd4: BIOS 0xb0:0x14 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's StopPAD (table B, 0x14): stop the BIOS's pad handler, which also stops memory card access
 * (psx-spx). PadStop calls it, in main's unreachable tail. */
void StopPAD(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_STOP_PAD);
}
