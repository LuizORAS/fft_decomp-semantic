#include "fft/battle.h"
#include "psx/types.h"

/* Queue the Cursor Move sound (MAIN_SFX_CURSOR_MOVE) for this frame, as
 * battle_sound_set_effect_to_cancel does. */
void battle_sound_set_effect_to_cursor_move(void) {
    g_battle_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
}
