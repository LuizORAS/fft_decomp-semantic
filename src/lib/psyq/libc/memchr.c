#include "psx/abi_inline.h"
#include "psx/libc.h"

/* The BIOS's memchr (table A): the first of `size` bytes equal to `value`, or 0. sprintf uses it. */
void* memchr(const void* source, u8 value, int size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_MEMCHR);
}
