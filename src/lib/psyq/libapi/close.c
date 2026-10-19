/* 0x80022094: BIOS 0xb0:0x36 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's close (table B, 0x36): close the file `fd`. card_file_close_with_retries and
 * world_card_close_file_with_retries call it. */
s32 close(s32 fd) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_CLOSE);
}
