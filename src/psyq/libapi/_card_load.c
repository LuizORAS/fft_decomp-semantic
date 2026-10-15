/* 0x80028750: BIOS 0xa0:0xac tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _card_load (table A, 0xac): start reading the directory of the card in `port` into the BIOS's
 * buffers; a card event reports the end (psx-spx). card_io_load_with_retries and world_card_load_with_retries
 * call it. */
s32 _card_load(s32 port) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_CARD_LOAD);
}
