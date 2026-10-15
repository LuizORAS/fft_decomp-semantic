/* 0x80022074: BIOS 0xb0:0x34 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's read (table B, 0x34): read `size` bytes of the file `fd` at its position into destination and
 * return how many were read. The memory card screens read saves with it, seeking back and trying again after
 * a short read (card_file_read_with_retries, world_card_read_file_with_retries and the chunked reader). */
s32 read(s32 fd, void* destination, s32 size) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_READ);
}
