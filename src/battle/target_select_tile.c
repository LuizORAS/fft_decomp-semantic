#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_camera.h"
#include "fft/battle_menu.h"
#include "fft/battle_target.h"
#include "psx/types.h"

/*
 * Seed the source unit's pending action target and run target-tile selection.
 *
 * During ability-preview phase 2 the target is the unit's own tile; for
 * player-controlled units it is the current cursor tile.
 * target_calculate_for_menu_types's result
 * picks the next step: 0/1 enter the ability-preview state, 2 restores the
 * unit and stops game flow, and -1 returns to the idle action menu.
 */
void target_select_tile(void) {
    battle_unit_t* unit;
    s32 result;
    s32 prev;

    unit = battle_unit_get_source_misc_data();
    unit->state_frame_counter = 0;
    if (unit->ability_preview_phase == TARGET_PANELS_NO_TARGETING) {
        main_util_set_svector(
            &unit->command_state.ai.data.action.target_position, unit->map_x, unit->map_z, unit->map_y);
        unit->command_state.ai.data.action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
    } else if (unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
        main_util_set_svector(&unit->command_state.ai.data.action.target_position, g_battle_cursor_x, g_battle_cursor_z,
            g_battle_cursor_y);
        unit->command_state.ai.data.action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
    }
    result = target_calculate_for_menu_types(&unit->command_state.ai.data.action);
    unit->target_select_result = result;
    switch (result) {
    case 0:
    case 1:
        camera_enable_map_pan();
        prev = g_battle_cursor_input_mode;
        g_battle_game_state = BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING;
        g_battle_cursor_input_mode = BATTLE_CURSOR_INPUT_FREE_CURSOR;
        g_battle_cursor_input_mode_saved = prev;
        target_set_tile_background_color(BATTLE_TILE_MARK_SHOW_TARGETS, 3);
        target_store_cursor_casting_unit_name_and_data();
        if (g_main_game_options.fields.target_flashing == GAME_OPTION_ON) {
            battle_gfx_tint_all_units_by_team();
        }
        break;
    case 2:
        camera_disable_map_pan();
        battle_state_enter_pre_attack_animation();
        target_store_cursor_casting_unit_name_and_data();
        break;
    case -1:
        battle_menu_dispatch_idle_action_menu();
        break;
    }
    action_clear_at_list_active();
}
