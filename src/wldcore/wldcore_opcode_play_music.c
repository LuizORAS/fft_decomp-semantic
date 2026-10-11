#include "fft/wldcore.h"
#include "psx/types.h"

void wldcore_opcode_play_music(void) {
    wldcore_sound_change_scenario_music((u8)(g_wldcore_active_saved_record.instruction >> 8));
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
