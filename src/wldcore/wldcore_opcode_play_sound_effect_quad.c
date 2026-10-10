#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound-novel opcode: stop the weather sound and play sound operand 0. A
 * nonzero operand 1 also records the sound so a restored reading replays
 * it. */
/* Target 0x8008c7ac. */
void wldcore_opcode_play_sound_effect_quad(void) {
    char unused[8];
    s32 sound_id;

    sound_id = g_wldcore_active_saved_record.instruction.bytes.operand_0;
    if (g_wldcore_opcode_state.instruction.word & 0xFF0000) {
        g_wldcore_active_saved_record.sound_id = sound_id;
    }
    wldcore_sound_stop_weather_sfx_and_send_quad(sound_id);
    g_wldcore_active_saved_record.state_flags |= WLDCORE_OPCODE_STATE_FETCH;
}
