#include "fft/battle.h"

/* Enter TARGETING_MESSAGE after the ability's target panels are built: open the "specify a
 * target" message, or the cannot-execute message when no panel is in range (preview phase 3),
 * and show the map cursor for a player-controlled unit (hide it for an AI unit). */
void battle_state_enter_targeting_message(void) {
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_TARGETING_MESSAGE;
    unit = battle_unit_get_source_misc_data();
    if (unit->ability_preview_phase == 3)
        battle_menu_init_system_function(
            1, 1, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
    else
        battle_menu_init_system_function(
            1, 0, unit->battle_data->misc_unit_id, 0, unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
    if (unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED)
        battle_target_show_cursor();
    else
        battle_target_hide_cursor();
}
