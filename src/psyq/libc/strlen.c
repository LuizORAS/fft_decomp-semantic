#include "psx/abi_inline.h"
#include "psx/libc.h"

/* The BIOS's strlen (table A): the length of a string. sprintf and FntPrint use it. */
int strlen(const char* source) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_STRLEN);
}
