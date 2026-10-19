/* 0x80021fc4: BIOS 0xb0:0x0d tail veneer. */
#include "psx/abi_inline.h"
#include "psx/libapi.h"

/* The BIOS's DisableEvent (table B, 0x0d): turn off the handling of `event`. The sound driver turns its root
 * counter 2 event off while it changes the music records (suzuki_smd_insert_music, suzuki_sound_stop_all_sfx and
 * others) and on again with EnableEvent; SpuQuit turns off LIBSPU's DMA event. */
s32 DisableEvent(s32 event) {
    PSYQ_BIOS_TAIL_CALL(PSYQ_BIOS_TABLE_B, PSYQ_BIOS_B_DISABLE_EVENT);
}
