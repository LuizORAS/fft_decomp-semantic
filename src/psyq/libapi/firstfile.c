/* 0x800220b4: BIOS 0xb0:0x42 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's firstfile (table B, 0x42): find the first file matching `pattern` (wildcards allowed), fill
 * `entry` and return it, or 0 (psx-spx). card_file_enumerate_saves and world_card_count_save_files walk a
 * card's saves with it and nextfile. */
DIRENTRY* firstfile(char* pattern, DIRENTRY* entry) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_FIRSTFILE);
}
