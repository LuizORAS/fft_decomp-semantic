/* 0x80021f24: BIOS 0xa0:0x44 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's FlushCache (table A, 0x44): flush the instruction cache, so code changed in RAM is fetched
 * again (psx-spx). Only the BIOS patches call it (_patch_card, _patch_card2, _patch_gte and
 * psyq_card_restore_exception_prefix); nothing calls it after loading an overlay. */
void FlushCache(void) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_FLUSH_CACHE);
}
