/* 0x800222fc: BIOS 0xa0:0x2b tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's memset (table A, 0x2b): fill `size` bytes at `destination` with `value` and return
 * `destination` (psx-spx). About 30 menu, card and text routines and FntLoad clear their buffers with it. */
void* memset(void* destination, u8 value, int size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_MEMSET);
}
