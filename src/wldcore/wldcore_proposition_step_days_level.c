#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the days level: Circle keeps the number (g_wldcore_proposition_dispatch_days) and Cross
 * gives up, both closing the number panel; once its thread ends the two levels close and the
 * dispatch is confirmed (wldcore_proposition_push_dispatch_confirm_level), or the list on offer
 * returns. */
void wldcore_proposition_step_days_level(wldcore_menu_panel_level_t* level) {
    if (level->result != 0 && thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
        g_wldcore_menu_blocked_buttons = 0;
        g_wldcore_menu_ordering_table_offset = 1;
        g_wldcore_window_render_record_count--;
        g_wldcore_window_render_object_count--;
        g_wldcore_menu_stack_depth -= 2;
        if (level->result == 1) {
            wldcore_proposition_push_dispatch_confirm_level();
        } else {
            wldcore_proposition_open_available_list();
        }
        return;
    }
    if (g_wldcore_input_new_button_presses & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        g_wldcore_context_value_display_mode = WLDCORE_VALUE_DISPLAY_FUNDS;
        level->result = -1;
        thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
    } else if (g_wldcore_input_new_button_presses & PSX_PAD_CIRCLE) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        level->result = 1;
        g_wldcore_proposition_dispatch_days = g_wldcore_active_menu_value;
        thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
    }
}
