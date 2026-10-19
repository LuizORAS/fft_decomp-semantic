#include "fft/battle.h"
#include "fft/battle_action.h"
#include "fft/battle_target.h"
#include "psx/pad.h"

/* ABILITY_PREVIEW_HANDLING: the preview of the ability on the chosen tile. Under player
 * control the camera can rotate, zoom and tilt, Circle asks for confirmation
 * (battle_state_enter_confirm_action) and Cross returns to target selection, or to the action menu for an
 * ability aimed at the unit's own tile (preview phase 2); an AI unit confirms after 31 frames. */
void battle_state_handle_ability_preview_state(void) {
    u16 frame_data;
    battle_unit_t* unit;

    unit = battle_unit_get_source();
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
        battle_camera_handle_rotation_input();
        battle_camera_handle_zoom_input();
        battle_camera_handle_tilt_input();
        if ((g_main_input_pressed_buttons & PSX_PAD_CIRCLE) != 0) {
            g_battle_cursor_input_mode = g_battle_cursor_input_mode_saved;
            battle_gfx_update_unit_palettes();
            battle_state_enter_confirm_action();
        } else if ((g_main_input_pressed_buttons & PSX_PAD_CROSS) != 0) {
            g_battle_cursor_input_mode = g_battle_cursor_input_mode_saved;
            battle_target_tint_marked_tiles(BATTLE_TILE_MARK_HIDE_TARGETS, 3);
            if (unit->ability_preview_phase == BATTLE_TARGET_PANELS_NO_TARGETING) {
                battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 3);
                battle_gfx_update_unit_palettes();
                battle_menu_open_active_unit_idle_action_menu();
            } else {
                battle_target_tint_marked_tiles(BATTLE_TILE_MARK_NONE, 3);
                battle_gfx_update_unit_palettes();
                battle_target_begin_tile_selection();
            }
        }
    } else {
        frame_data = unit->state_frame_counter;
        unit->state_frame_counter = frame_data + 1;
        if (frame_data >= 0x1f) {
            g_battle_cursor_input_mode = g_battle_cursor_input_mode_saved;
            battle_gfx_update_unit_palettes();
            battle_state_enter_confirm_action();
        }
    }
    battle_state_update_units();
    battle_menu_update_menus_and_event_speed(main_gfx_get_otag(), g_main_input_current_buttons);
}
