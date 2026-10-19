/* 0x800222bc: BIOS 0xa0:0x17 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"
#include "psx/libc.h"

/* The BIOS's strcmp (table A, 0x17): compare two strings: 0 when equal, otherwise the difference of the first
 * bytes that differ (psx-spx). card_save_update_slot_scan and world_card_update_slot_scan compare save names
 * with it. */
s32 strcmp(const char* left, const char* right) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_STRCMP);
}
