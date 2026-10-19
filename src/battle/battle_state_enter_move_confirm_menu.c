#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "fft/battle_move.h"
#include "fft/battle_text.h"
#include "fft/battle_unit.h"

/* Enter MOVE_CONFIRM_MENU once the unit has stopped moving and show the matching prompt, with a
 * one-second message time.
 *
 * A failed Teleport shows effect message 0x183b; a successful one the move confirmation. Other movement
 * tints the unit by team and asks for confirmation, silently when navigation messages are off. The map
 * cursor shows for a player-controlled unit and hides otherwise. */
void battle_state_enter_move_confirm_menu(void) {
    battle_unit_t* source;
    battle_unit_t* caster;

    battle_camera_disable_map_pan();
    g_battle_game_state = BATTLE_GAME_STATE_MOVE_CONFIRM_MENU;
    source = battle_unit_get_source();
    caster = battle_unit_get_casting();
    battle_text_set_message_duration_frames(0x3C);
    if (caster->path.flags & BATTLE_EFFECTIVE_MOVEMENT_TELEPORT) {
        if (caster->last_path_count == BATTLE_MOVE_PATH_TELEPORT_FAILED) {
            battle_menu_init_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_EFFECT_MESSAGE, 0x183B, source->stats->battle_id, 0, 1);
        } else {
            battle_menu_init_system_function(
                BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM, 0, source->stats->battle_id, 0, 0);
        }
        caster->last_path_count = 0;
    } else {
        if (g_main_game_options.fields.navigation_messages != GAME_OPTION_ON) {
            battle_gfx_modify_palette_based_on_team(source, 2);
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM_SILENT, 0,
                source->stats->battle_id, 0, source->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        } else {
            battle_gfx_modify_palette_based_on_team(source, 2);
            battle_menu_init_system_function(BATTLE_MENU_SYSTEM_COMMAND_MOVE_CONFIRM, 0, source->stats->battle_id, 0,
                source->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED);
        }
    }
    if (source->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) {
        battle_target_show_cursor();
    } else {
        battle_target_hide_cursor();
    }
}
