/* 0x80021f54: BIOS 0xa0:0x9f tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libcd.h"

/* The BIOS's SetMem (table A, 0x9f): set the RAM size the memory controller decodes to `mode` megabytes
 * (psx-spx). main_file_init_cd sets 2, the console's actual RAM, at boot and on each reset. */
void SetMem(int mode) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_SET_MEM);
}
