/* 0x80021f74: BIOS 0xb0:0x08 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's OpenEvent (table B, 0x08): add an event of class `desc` and kind `spec` to the BIOS's table,
 * disabled, and return its descriptor, or -1 when the table is full. With `mode` 0x1000 the BIOS calls
 * `callback` when the event fires; with 0x2000 it only marks the event ready (psx-spx). The memory card events
 * (main_card_init_events), LIBSPU's DMA event and the sound driver's root counter 2 event open this way. */
s32 OpenEvent(u32 desc, s32 spec, s32 mode, psyq_api_event_callback_t callback) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_OPEN_EVENT);
}
