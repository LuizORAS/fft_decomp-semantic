#include "fft/wldcore.h"
#include "psx/types.h"

/* Sound novel opcode 29: take the numeric value display down again
 * (wldcore_sound_novel_show_value_display puts it up) and fetch the next instruction. */
void wldcore_sound_novel_hide_value_display(void) {
    g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
    g_wldcore_sound_novel_record.state_flags
        = (g_wldcore_sound_novel_record.state_flags & ~WLDCORE_SOUND_NOVEL_STATE_VALUE_DISPLAY)
        | WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
