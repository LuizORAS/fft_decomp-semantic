/* 0x80021fb4: BIOS 0xb0:0x0c tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's EnableEvent (table B, 0x0c): turn on the handling of `event`. The memory card events, LIBSPU's
 * DMA event and the sound driver's root counter 2 event are enabled this way. */
s32 EnableEvent(s32 event) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_ENABLE_EVENT);
}
