/* 0x800220d4: BIOS 0xb0:0x45 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's erase (table B, 0x45): delete the file `path`; returns 1 when it did (psx-spx).
 * card_file_delete_selected_with_retries and world_card_delete_file call it. */
s32 erase(const char* path) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_DELETE);
}
