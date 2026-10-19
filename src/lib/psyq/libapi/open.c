/* 0x80022054: BIOS 0xb0:0x32 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's open (table B, 0x32): open the file `path` with `mode` and return its descriptor, or -1. The
 * memory card screens open saves on "bu00:" (slot 1) or "bu10:" (slot 2) this way
 * (card_file_open_selected_with_retries, world_card_open_file_with_retries). */
s32 open(const char* path, u32 mode) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_OPEN);
}
