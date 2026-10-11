#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_opcode_set_sound_mode(void) {
    u16 flags;

    flags = g_wldcore_state_flags | WLDCORE_OPCODE_STATE_FETCH;
    g_wldcore_active_saved_record.sound_mode = (u8)(g_wldcore_active_saved_record.instruction >> 8);
    g_wldcore_state_flags = flags;
}
