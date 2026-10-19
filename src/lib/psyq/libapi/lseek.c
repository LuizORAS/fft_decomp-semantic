/* 0x80022064: BIOS 0xb0:0x33 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's lseek (table B, 0x33): move the file `fd`'s position to `offset` from the start (`origin` 0)
 * or from the current position (1) and return the new position. The card readers and writers take the current
 * position with (0, 1) and seek back to it for a retry (card_file_seek_with_retries,
 * world_card_seek_file_with_retries). */
s32 lseek(s32 fd, s32 offset, s32 origin) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_SEEK);
}
