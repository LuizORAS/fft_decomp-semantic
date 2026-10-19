/* 0x800220c4: BIOS 0xb0:0x43 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's nextfile (table B, 0x43): the next file matching firstfile's pattern, into `entry`; returns it,
 * or 0 at the end. */
DIRENTRY* nextfile(DIRENTRY* entry) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_NEXTFILE);
}
