#include "fft/battle.h"
#include "psx/pad.h"

/* ABILITY_PREVIEW_HANDLING: the preview of the ability on the chosen tile. Under player
 * control the camera can rotate, zoom and tilt, Circle asks for confirmation
 * (battle_action_confirm) and Cross returns to target selection, or to the action menu for an
 * ability aimed at the unit's own tile (preview phase 2); an AI unit confirms after 31 frames. */
void battle_state_handle_ability_preview_state(void) {
    u16 frame_data;
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_source_misc_data();
    if ((unit->team_flags & BATTLE_TEAM_FLAG_PLAYER_CONTROLLED) != 0) {
        battle_camera_handle_rotation_input();
        battle_camera_call_zoom_map();
        battle_camera_call_toggle_tilt();
        if ((g_controller_input_pressed & PSX_PAD_CIRCLE) != 0) {
            g_battle_controller_input = g_controller_input_copy_12;
            battle_gfx_update_unit_palettes();
            battle_action_confirm();
        } else if ((g_controller_input_pressed & PSX_PAD_CROSS) != 0) {
            g_battle_controller_input = g_controller_input_copy_12;
            battle_target_set_tile_background_color(8, 3);
            if (unit->ability_preview_phase == 2) {
                battle_target_set_tile_background_color(0, 3);
                battle_gfx_update_unit_palettes();
                battle_menu_open_active_unit_idle_action_menu();
            } else {
                battle_target_set_tile_background_color(0, 3);
                battle_gfx_update_unit_palettes();
                battle_target_set_boxes_red();
            }
        }
    } else {
        frame_data = unit->state_frame_counter;
        unit->state_frame_counter = frame_data + 1;
        if (frame_data >= 0x1f) {
            g_battle_controller_input = g_controller_input_copy_12;
            battle_gfx_update_unit_palettes();
            battle_action_confirm();
        }
    }
    battle_state_update_units();
    battle_menu_draw_selection_data(main_gfx_get_otag(), g_controller_input_raw);
}
