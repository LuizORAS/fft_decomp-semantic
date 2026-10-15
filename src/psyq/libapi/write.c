/* 0x80022084: BIOS 0xb0:0x35 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's write (table B, 0x35): write `size` bytes from source to the file `fd` at its position and
 * return how many were written. The memory card screens write saves with it (card_file_write_with_retries,
 * world_card_write_file_with_retries and the buffer writers). */
s32 write(s32 fd, const void* source, s32 size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_WRITE);
}
