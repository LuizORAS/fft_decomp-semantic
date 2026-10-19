/* 0x80028740: BIOS 0xa0:0xab tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's _card_info (table A, 0xab): start checking the card in `port` (0x00 slot 1, 0x10 slot 2); a
 * card event reports the result: done, error, timeout or a new card. The callers treat 1 as started. The
 * CARD, OPEN and WORLD card code use it to find whether a card is inserted and usable. */
s32 _card_info(s32 port) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_A, PSYQ_BIOS_A_CARD_INFO);
}
