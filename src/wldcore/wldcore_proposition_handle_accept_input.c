#include "fft/wldcore.h"
#include "psx/pad.h"

/* Menu input step: cancel (0x40) closes this level and returns through
 * 0x80072a18; confirm (0x20) stores the 0x8009f2f4 + 0x8009f2f6 total, shows
 * message 0xb810 (with the total substituted) or 0xb80f when it is zero,
 * opens the 0x80073778 panel at (0x48, 0x20), and pushes a level tagged 0xa. */
void wldcore_proposition_handle_accept_input(wldcore_menu_window_render_level_t* owner) {
    s32 total;
    wldcore_point32_t origin;

    if (g_wldcore_window_render_records[owner->render_index].flags & 0x100) {
        return;
    }
    if (g_wldcore_new_button_presses & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        g_wldcore_window_render_record_count--;
        g_wldcore_window_render_object_count--;
        g_wldcore_menu_stack_depth--;
        wldcore_list_open_available_propositions();
    } else if (g_wldcore_new_button_presses & PSX_PAD_CIRCLE) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        total = g_wldcore_selected_proposition_row[0].fields.fee_a + g_wldcore_selected_proposition_row[0].fields.fee_b;
        g_wldcore_proposition_gil_amount = total;
        if (total != 0) {
            g_world_text_substitution_values[0] = total;
            world_thread_set_parameters(0xE, 0x19, 0xB810, 0);
        } else {
            world_thread_set_parameters(0xE, 0x19, 0xB80F, 0);
        }
        wldcore_window_set_render_state_2_from_arg_0x04((s32*)owner);
        origin.x = 0x48;
        origin.y = 0x20;
        wldcore_window_build_yes_no_panel(origin);
        g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_ACCEPT;
        g_wldcore_menu_stack_depth++;
    }
}
