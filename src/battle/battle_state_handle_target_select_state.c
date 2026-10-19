#include "fft/battle.h"
#include "fft/battle_target.h"
#include "psx/pad.h"

/* TARGET_SELECT: choose the casting unit's Auto-Battle target. The camera can rotate, zoom and
 * tilt and the d-pad moves the cursor; Circle on a unit of the other team (command 0xc) or of
 * the same team (0xe) asks for confirmation, and any other tile or unit is refused. Cross
 * returns to the action menus and Triangle recentres the cursor on the caster. */
void battle_state_handle_target_select_state(void) {
    battle_unit_t* caster;
    battle_unit_t* target;

    battle_camera_handle_rotation_input();
    battle_camera_handle_zoom_input();
    battle_camera_handle_tilt_input();
    battle_target_move_cursor_by_input();
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    if (g_main_input_pressed_buttons & PSX_PAD_CIRCLE) {
        caster = battle_unit_get_casting();
        target = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
        /* The targeted choice requires an enemy, the recovery choice an ally. */
        if (target == 0) {
            battle_state_enter_target_select_denied();
        } else if (caster->target_select_command == BATTLE_AI_COMMAND_AUTOBATTLE_TARGETED_SELECT) {
            if ((caster->stats->team_flags & BATTLE_TEAM_MASK) == (target->stats->team_flags & BATTLE_TEAM_MASK)) {
                battle_state_enter_target_select_denied();
            } else {
                battle_state_enter_target_select_confirm();
            }
        } else if (caster->target_select_command == BATTLE_AI_COMMAND_AUTOBATTLE_RECOVERY_SELECT) {
            if ((caster->stats->team_flags & BATTLE_TEAM_MASK) == (target->stats->team_flags & BATTLE_TEAM_MASK)) {
                battle_state_enter_target_select_confirm();
            } else {
                battle_state_enter_target_select_denied();
            }
        }
        battle_gfx_update_unit_palettes();
    } else if (g_main_input_pressed_buttons & PSX_PAD_CROSS) {
        battle_target_move_cursor_to_unit(battle_unit_get_casting());
        battle_gfx_update_unit_palettes();
        battle_state_enter_open_action_menus();
    } else if (g_main_input_pressed_buttons & PSX_PAD_TRIANGLE) {
        battle_target_move_cursor_to_unit(battle_unit_get_casting());
    }
}
