#include "fft/battle.h"

/* Start the source unit's AI turn (AI_COMMAND): reset its state counter and command readiness,
 * clear the tile colours and store its names for the menus. */
void battle_state_enter_ai_command(void) {
    battle_unit_misc_data_t* unit;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_AI_COMMAND;
    unit = battle_unit_get_source_misc_data();
    unit->state_frame_counter = 0;
    unit->command_ready = 0;
    battle_target_tint_marked_tiles(BATTLE_TARGET_TINT_RESET, 0);
    if (unit != 0 && unit->battle_data != 0) {
        battle_menu_store_unit_names_and_event_block_data(3, unit->battle_data->misc_unit_id, 0);
    }
}
