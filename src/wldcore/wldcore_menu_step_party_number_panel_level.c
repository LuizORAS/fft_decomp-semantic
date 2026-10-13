#include "fft/wldcore.h"
#include "psx/pad.h"

/* Input step of the Party number panel: Circle or Cross closes it. Once the panel thread has ended
 * the level pops and, after Circle with a number below 0x200, the party is rebuilt from that ENTD
 * (main_entd_init_event_unit_data), the formation names are regenerated and the map redraws; the level
 * underneath resumes either way.
 *
 * level->result is 0 while waiting, 1 on confirm and -1 on cancel. */
void wldcore_menu_step_party_number_panel_level(wldcore_menu_panel_level_t* level) {
    u32 buttons;

    if ((level->result != 0) && (world_thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0)) {
        g_wldcore_menu_blocked_buttons = 0;
        g_wldcore_menu_ordering_table_offset = 1;
        g_wldcore_menu_stack_depth--;
        if (level->result == 1) {
            if (g_wldcore_active_menu_value < 0x200) {
                main_entd_init_event_unit_data(g_wldcore_active_menu_value);
                world_text_generate_formation_unit_name_string();
                g_main_system_flags |= MAIN_SYSTEM_FLAG_DRAW_MAP;
            }
        }
        wldcore_menu_dispatch_resume_handler();
        return;
    }

    buttons = g_wldcore_new_button_presses;
    if ((buttons & PSX_PAD_CROSS) != 0) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        level->result = -1;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
    } else if ((buttons & PSX_PAD_CIRCLE) != 0) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        level->result = 1;
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
    }
}
