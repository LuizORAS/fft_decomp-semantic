#include "fft/battle.h"

/* Enter CONTINUE_TURN at 60 fps and reset the source unit's counter. */
void battle_state_enter_continue_turn(void) {
    battle_unit_misc_data_t* unit;

    g_battle_state_vsync_interval = 1;
    g_battle_game_state = BATTLE_GAME_STATE_CONTINUE_TURN;
    unit = battle_unit_get_source_misc_data();
    if (unit != 0) {
        unit->state_frame_counter = 0;
    }
}
