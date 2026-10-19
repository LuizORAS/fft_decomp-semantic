/* 0x80026f94: BIOS 0xa0:0x2a tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's memcpy (table A, 0x2a) under a library name: copy `size` bytes from `source` to `destination`.
 * LIBGPU's environment calls (GetDrawEnv, PutDrawEnv, GetDispEnv, PutDispEnv, DrawOTagEnv) copy their
 * records with it. */
void* memcpy(void* destination, const void* source, unsigned int size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_MEMCPY);
}
