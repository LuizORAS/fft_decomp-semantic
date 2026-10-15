#include "fft/battle.h"
#include "psx/types.h"

/* Switch to 30 fps and the EFFECT state, and show the map cursor. Nothing calls it or
 * battle_state_enter_effect_playback, the only other code that enters EFFECT, so that state
 * never runs (QUIRKS.md). */
void battle_state_enter_effect_state(void) {
    g_main_system_vsync_interval = 2;
    g_battle_game_state = BATTLE_GAME_STATE_EFFECT;
    battle_target_show_cursor();
}
