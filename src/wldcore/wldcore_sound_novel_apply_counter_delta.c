#include "fft/wldcore.h"

/* Sound novel opcode 30: add the pending delta to the record's counter, clamped to 0..0xFFFF, clear
 * the delta and show the new counter. */
void wldcore_sound_novel_apply_counter_delta(void) {
    s32 delta;
    s32 current;
    s32* value = &g_wldcore_sound_novel_record.counter;

    delta = g_wldcore_sound_novel_record.counter_delta;
    g_wldcore_sound_novel_record.counter_delta = 0;
    current = *value;
    *value = current + delta;
    if (*value > 0xFFFF) {
        *value = 0xFFFF;
    }
    if (*value < 0) {
        *value = 0;
    }
    g_wldcore_displayed_numeric_value = *value;
    g_wldcore_sound_novel_record.state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
}
