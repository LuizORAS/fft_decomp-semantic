#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_target.h"
#include "psx/pad.h"

/* FREE_CURSOR state, also any unknown state: camera and cursor input, then the button actions
 * (help, team highlight, act menu, move range, AT list and mini menu). */
void battle_state_handle_free_cursor_state(void) {
    battle_unit_t* unit;
    s32 input;
    RECT rect; /* never used; retail reserves its 8 bytes */

    battle_camera_handle_rotation_input();
    battle_camera_handle_zoom_input();
    battle_camera_handle_tilt_input();
    battle_target_move_cursor_by_input();
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
    input = g_main_input_pressed_buttons;
    if (input & PSX_PAD_SELECT) {
        battle_turn_clear_at_list_index();
        battle_state_enter_free_cursor_help();
    } else if (input & PSX_PAD_SQUARE) {
        battle_turn_clear_at_list_index();
        battle_state_enter_highlight_units_by_team();
    } else if (input & PSX_PAD_CIRCLE) {
        battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 0);
        main_sound_play_sfx(MAIN_SFX_CONFIRM);
        if (battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z) != 0) {
            battle_menu_dispatch_idle_action_menu();
        } else {
            unit = battle_unit_get_source();
            if (unit != 0) {
                battle_target_move_cursor_to_unit(unit);
                battle_state_enter_open_action_menus();
            }
        }
    } else if (input & PSX_PAD_CROSS) {
        battle_turn_clear_at_list_index();
        unit = battle_unit_find_selectable_at_tile(g_battle_cursor_x, g_battle_cursor_y, g_battle_cursor_z);
        if (unit != 0) {
            g_battle_casting_runtime_id = unit->runtime_id;
            battle_target_show_move_range_in_free_cursor();
        }
    } else if (input & PSX_PAD_START) {
        if (g_main_game_options.fields.multi_height_cursor_speed != GAME_MULTI_HEIGHT_CURSOR_SPEED_STOP) {
            main_sound_play_sfx(MAIN_SFX_CONFIRM);
            battle_turn_show_next_at_entry();
            battle_target_show_cursor_unit_panel();
        }
    } else if (input & PSX_PAD_TRIANGLE) {
        battle_turn_clear_at_list_index();
        main_sound_play_sfx(MAIN_SFX_CONFIRM);
        battle_state_enter_mini_menu();
    }
}
