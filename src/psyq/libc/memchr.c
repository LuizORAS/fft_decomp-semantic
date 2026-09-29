#include "psx/abi_inline.h"
#include "psx/libc.h"
void* memchr(const void* source, int value, unsigned int size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_MEMCHR);
}
