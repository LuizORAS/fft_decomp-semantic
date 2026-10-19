/* 0x800220a4: BIOS 0xb0:0x41 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's format (table B, 0x41): format the card `path` names, erasing every file; returns 1 when it did
 * (psx-spx). card_io_format_selected_slot and world_card_format_selected_slot call it. */
s32 format(const char* path) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FILE_FORMAT);
}
