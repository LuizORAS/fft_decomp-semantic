#include "fft/battle.h"

/* Enter the JP/EXP gain state at animation speed 1 and restart the source unit's
 * state frame counter. */
void battle_state_enter_jp_xp_gain(void) {
    battle_unit_t* unit;

    g_main_system_animation_speed = 1;
    g_battle_game_state = BATTLE_GAME_STATE_JP_EXP_GAIN;
    unit = battle_unit_get_source();
    if (unit != 0) {
        unit->state_frame_counter = 0;
    }
}
