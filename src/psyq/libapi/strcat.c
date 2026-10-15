/* 0x800222ac: BIOS 0xa0:0x15 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's strcat (table A, 0x15): append `source` to `destination` and return `destination`. The card
 * screens build a save's path, the slot device then the file name, with strcpy and it. */
char* strcat(char* destination, const char* source) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_STRCAT);
}
