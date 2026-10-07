#include "fft/battle.h"
#include "psx/types.h"

/* Take the chosen tile as the action's target and mark the area it hits
 * (battle_target_mark_action_area): the unit's own tile when no choice is needed (phase 2), else the
 * cursor tile for a player-controlled unit (an AI unit's target is already set). On 0 or 1 enter
 * ABILITY_PREVIEW_HANDLING with the hit tiles tinted, the acting unit and its target in the status
 * panels and, with the Target Flashing option, the units tinted by team; on 2 go straight to the
 * charge animation (PRE_ATTACK_ANIMATION); on -1 return to the action menu. The cursor is hidden
 * either way. */
void battle_target_select_tile(void) {
    battle_unit_misc_data_t* unit;
    s32 result;
    s32 prev;

    unit = battle_unit_get_source_misc_data();
    unit->state_frame_counter = 0;
    if (unit->ability_preview_phase == 2) {
        main_util_set_svector(
            (SVECTOR*)&unit->command_state.ai.data.action.target_x, unit->map_x, unit->map_z, unit->map_y);
        unit->command_state.ai.data.action.targeting_type = 5;
    } else if (unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
        main_util_set_svector((SVECTOR*)&unit->command_state.ai.data.action.target_x, g_battle_cursor_x,
            g_battle_cursor_z, g_battle_cursor_y);
        unit->command_state.ai.data.action.targeting_type = 5;
    }
    result = battle_target_mark_action_area(&unit->command_state.ai.data.action.unit_id);
    unit->target_select_result = result;
    switch (result) {
    case 0:
    case 1:
        battle_state_enable_camera_pan();
        prev = g_battle_controller_input;
        g_battle_game_state = BATTLE_GAME_STATE_ABILITY_PREVIEW_HANDLING;
        g_battle_controller_input = 2;
        g_controller_input_copy_12 = prev;
        battle_target_tint_marked_tiles(BATTLE_TARGET_TINT_TARGETED, 3);
        battle_target_show_actor_and_target_panels();
        if (g_main_game_options.fields.target_flashing == GAME_OPTION_ON) {
            battle_gfx_tint_all_units_by_team();
        }
        break;
    case 2:
        battle_state_disable_camera_pan();
        battle_state_enter_pre_attack_animation();
        battle_target_show_actor_and_target_panels();
        break;
    case -1:
        battle_menu_dispatch_idle_action_menu();
        break;
    }
    battle_target_hide_cursor();
}
