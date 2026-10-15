#include "fft/battle.h"
#include "psx/types.h"

/* Enter MOVE_CONFIRM_MENU once the unit has stopped moving and show the matching prompt, with a
 * one-second message time.
 *
 * A failed Teleport shows effect message 0x183b; a successful one the move confirmation. Other movement
 * tints the unit by team and asks for confirmation, silently when navigation messages are off. The map
 * cursor shows for a player-controlled unit and hides otherwise. */
void battle_state_enter_move_confirm_menu(void) {
    battle_unit_misc_data_t* source;
    battle_unit_misc_data_t* caster;

    battle_state_disable_camera_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MOVE_CONFIRM_MENU;
    source = battle_unit_get_source_misc_data();
    caster = battle_unit_get_casting_misc_data();
    battle_text_set_message_duration_frames(0x3C);
    if (caster->movement_flags & BATTLE_EFFECTIVE_MOVEMENT_TELEPORT) {
        if (caster->last_path_count == BATTLE_MOVE_PATH_TELEPORT_FAILED) {
            battle_menu_init_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0x183B, source->battle_data->misc_unit_id, 0, 1);
        } else {
            battle_menu_init_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM, 0, source->battle_data->misc_unit_id, 0, 0);
        }
        caster->last_path_count = 0;
    } else {
        if (g_main_game_options.fields.navigation_messages != GAME_OPTION_ON) {
            battle_gfx_modify_palette_based_on_team(source, 2);
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM_SILENT, 0,
                source->battle_data->misc_unit_id, 0, source->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_gfx_modify_palette_based_on_team(source, 2);
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM, 0,
                source->battle_data->misc_unit_id, 0, source->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
    }
    if (source->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
        battle_target_show_cursor();
    } else {
        battle_target_hide_cursor();
    }
}
