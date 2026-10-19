/* 0x80021f84: BIOS 0xb0:0x09 tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's CloseEvent (table B, 0x09): release `event` from the BIOS's table. SpuQuit and suzuki_sound_quit
 * close their events with it. */
s32 CloseEvent(s32 event) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_CLOSE_EVENT);
}
