/* 0x800287b8: BIOS 0xb0:0x4e tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _card_write (table B, 0x4e): start writing the 128-byte sector `sector` of the card in `port`
 * from `data`; returns 1, or 0 for a bad sector number (psx-spx). Only _card_clear calls it, for a dummy
 * write to sector 0x3f with `data` 0. */
s32 _card_write(s32 port, s32 sector, const void* data) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_CARD_WRITE);
}
