/* 0x800220e4: BIOS 0xb0:0x55 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _get_error (table B, 0x55): the error code of the file `fd`, -1 for a bad descriptor
 * (psx-spx). The memory card readers and writers give up when it is not 0. */
s32 _get_error(s32 fd) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_GET_ERROR);
}
