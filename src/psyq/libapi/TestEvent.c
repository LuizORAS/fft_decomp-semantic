/* 0x80021fa4: BIOS 0xb0:0x0b tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's TestEvent (table B, 0x0b): return 1 when `event` is ready, setting it busy again, or 0. The
 * memory card code polls its eight events this way (card_io_*, open_card_*, world_card_*). */
s32 TestEvent(s32 event) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_TEST_EVENT);
}
