#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Sound novel opcode 43: yield until Circle is pressed.
 *
 * Each block takes its own pointer local so the flag address is materialized twice, as in the
 * target. */
void wldcore_sound_novel_wait_for_circle_press(void) {
    if ((g_wldcore_input_new_button_presses & PSX_PAD_CIRCLE) != 0) {
        u16* state_flags;

        state_flags = &g_wldcore_sound_novel_record.state_flags;
        *state_flags |= WLDCORE_SOUND_NOVEL_STATE_FETCH;
    }
    {
        u16* state_flags;

        state_flags = &g_wldcore_sound_novel_record.state_flags;
        *state_flags |= WLDCORE_SOUND_NOVEL_STATE_YIELD;
    }
}
