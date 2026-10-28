/* 0x800222dc: BIOS 0xa0:0x27 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's bcopy (table A, 0x27): copy `size` bytes from `source` to `destination`, one byte at a time
 * (psx-spx: the ROM's copy and fill routines are slow). About 40 menu, card and overlay routines copy their
 * records with it. */
void* bcopy(const void* source, void* destination, int size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_BCOPY);
}
