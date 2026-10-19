/* 0x800222ec: BIOS 0xa0:0x28 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's bzero (table A, 0x28): fill `size` bytes at `destination` with 0 and return `destination`, or 0
 * for a size of 0 (psx-spx). The battle map loader clears its GNS records with it, and battle_map_load_data
 * compares that return value (QUIRKS.md). */
void* bzero(void* destination, s32 size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_BZERO);
}
