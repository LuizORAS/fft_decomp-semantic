#include "fft/wldcore.h"

/* Sound novel opcode 5: load background set operand 0 unless it is the one already recorded, waiting
 * for the file. */
void wldcore_sound_novel_load_background_set(void) {
    s32 value;

    value = g_wldcore_sound_novel_record.instruction.bytes.operand_0;
    if (g_wldcore_sound_novel_record.background_set != value) {
        DrawSync(0);
        g_wldcore_sound_novel_record.background_set = value;
        wldcore_gfx_load_background_set(value);
        wldcore_wait_for_file_load();
    }
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
