#include "fft/wldcore.h"
#include "psx/types.h"

/*
 * Runs the WORLD menu code for one world-map frame, drawing into `context`.
 *
 * Skipped while MAIN_SYSTEM_FLAG_SKIP_MENU_FRAME is set. While the unit panel
 * threads are in use it draws the window frames, runs the event input update
 * and plays the frame's sound effect itself; otherwise it runs the formation
 * menu frame. Buttons in g_wldcore_menu_blocked_buttons are withheld from
 * the menus.
 */
void wldcore_menu_run_world_frame(GsOT* context) {
    u32* otag_entries;
    u32* sprite_entries;
    u32* menu_entries;

    if (g_main_system_flags & MAIN_SYSTEM_FLAG_SKIP_MENU_FRAME) {
        return;
    }
    world_menu_set_zodiac_backdrop_otag(context->org + 15);
    world_menu_set_allowed_buttons(~g_wldcore_menu_blocked_buttons);
    if (g_main_system_flags & MAIN_SYSTEM_FLAG_UNIT_PANEL_THREADS) {
        otag_entries = context->org;
        g_world_gfx_active_otag_entries = otag_entries + g_wldcore_menu_ordering_table_offset;
        world_menu_draw_active_window_frames();
        sprite_entries = context->org;
        world_input_update_event_state(sprite_entries + g_wldcore_menu_ordering_table_offset + 2,
            g_wldcore_current_button_input, g_active_graphics_buffer_index * 240);
        if (g_world_sound_pending_sfx_id != -1) {
            wldcore_sound_play_effect(g_world_sound_pending_sfx_id);
        }
    } else {
        menu_entries = context->org;
        world_menu_run_frame(menu_entries + g_wldcore_menu_ordering_table_offset, g_wldcore_current_button_input);
    }
}
