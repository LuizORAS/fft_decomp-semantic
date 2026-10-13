#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of a proposition's detail. Cross returns to the list on offer; Circle asks whether to take the
 * job, with its fee (the row's two fee amounts, kept in g_wldcore_proposition_gil_amount) when it has
 * one, on a yes/no panel (WLDCORE_MENU_LEVEL_PROPOSITION_ACCEPT). */
void wldcore_proposition_handle_accept_input(wldcore_menu_window_render_level_t* owner) {
    s32 total;
    wldcore_point32_t origin;

    if (g_wldcore_window_render_records[owner->render_index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (g_wldcore_new_button_presses & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        g_wldcore_window_render_record_count--;
        g_wldcore_window_render_object_count--;
        g_wldcore_menu_stack_depth--;
        wldcore_proposition_open_available_list();
    } else if (g_wldcore_new_button_presses & PSX_PAD_CIRCLE) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        total = g_wldcore_selected_proposition_row[0].fields.fee_a + g_wldcore_selected_proposition_row[0].fields.fee_b;
        g_wldcore_proposition_gil_amount = total;
        if (total != 0) {
            g_world_text_substitution_values[0] = total;
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_JOB_FEE_PROMPT, 0);
        } else {
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_ACCEPT_JOB_PROMPT, 0);
        }
        wldcore_menu_suspend_proposition_detail_level((s32*)owner);
        origin.x = 0x48;
        origin.y = 0x20;
        wldcore_window_build_yes_no_panel(origin);
        g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_PROPOSITION_ACCEPT;
        g_wldcore_menu_stack_depth++;
    }
}
