#include "fft/battle_sound.h"

void battle_sound_set_effect_to_cursor_move(void) {
    g_battle_sound_pending_sfx_id = MAIN_SFX_CURSOR_MOVE;
}
