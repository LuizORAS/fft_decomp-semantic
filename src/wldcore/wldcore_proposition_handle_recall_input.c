#include "fft/thread.h"
#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the call-back question. Up and down switch the row; Cross and "no" return to the list of
 * propositions in progress. "Yes" calls the units back (WLDCORE_PROPOSITION_PROGRESS_RECALLED: they
 * return the next day, with nothing earned), the barkeeper confirms
 * (WLDCORE_TEXT_ID_CALL_BACK_ACCEPTED), and the next press returns to the list. */
void wldcore_proposition_handle_recall_input(wldcore_menu_stack_record_t* level) {
    s32 choice; /* 0x14; 0 = first row, 1 = second row */

    if (g_wldcore_window_render_records[level->proposition_cancel_confirm.choice_render].flags
        & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (level->proposition_cancel_confirm.confirmed != 0) {
        if (g_wldcore_input_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS)) {
            wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
            goto close;
        }
        return;
    }
    if (g_wldcore_input_new_button_presses & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        /* Shared tail: the target keeps this copy and the other close paths
         * jump into it; duplicated copies cross-jump into the last one. */
    close:
        g_wldcore_window_record_count -= 2;
        g_wldcore_window_render_record_count -= 2;
        g_wldcore_window_render_object_count -= 4;
        g_wldcore_menu_stack_depth--;
        wldcore_proposition_open_active_list();
        return;
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_UP) != 0
        || wldcore_input_check_repeating_directional(PSX_PAD_DOWN) != 0) {
        choice = level->proposition_cancel_confirm.choice ^ 1;
        level->proposition_cancel_confirm.choice = choice;
        g_wldcore_window_records[level->proposition_cancel_confirm.cursor_window].position.y
            = g_wldcore_window_render_records[level->proposition_cancel_confirm.choice_render].base.y
            - -(choice * 0x10 + 0xE);
        wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
    }
    if (!(g_wldcore_input_new_button_presses & PSX_PAD_CIRCLE)) {
        return;
    }
    if (level->proposition_cancel_confirm.confirmed != 0) {
        return;
    }
    wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
    if (level->proposition_cancel_confirm.choice == 1) {
        goto close;
    }
    thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CALL_BACK_ACCEPTED, 0);
    level->proposition_cancel_confirm.confirmed = 1;
    g_main_active_propositions[level->proposition_cancel_confirm.proposition].flags
        |= WLDCORE_PROPOSITION_PROGRESS_RECALLED;
    g_wldcore_window_records[level->proposition_cancel_confirm.cursor_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[level->proposition_cancel_confirm.frame_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[level->proposition_cancel_confirm.choice_render].flags
        |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
}
