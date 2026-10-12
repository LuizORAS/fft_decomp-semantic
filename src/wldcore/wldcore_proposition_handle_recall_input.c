#include "fft/wldcore.h"
#include "psx/pad.h"

/* The yes/no confirmation menu level this handler drives. The
 * level owns a cursor window (0x00), a render record (0x04), the proposition
 * it acts on (0x10), the highlighted answer (0x14), the confirmed flag
 * (0x18) and a second window (0x20). Only these words are touched here. */
typedef struct wldcore_menu_proposition_confirm_level {
    s32 window;        /* 0x00 */
    s32 render_index;  /* 0x04 */
    u8 _unused_08[8];  /* 0x08 */
    s32 proposition;   /* 0x10; index into g_main_active_propositions */
    s32 choice;        /* 0x14; 0 = first row, 1 = second row */
    s32 confirmed;     /* 0x18; set once the answer has been taken */
    u8 _unused_1c[4];  /* 0x1c */
    s32 second_window; /* 0x20 */
} wldcore_menu_proposition_confirm_level_t;

/* Step of the call-back question. Up and down switch the row; Cross and "no" return to the list of
 * propositions in progress. "Yes" calls the units back (WLDCORE_PROPOSITION_PROGRESS_RECALLED: they
 * return the next day, with nothing earned), the barkeeper confirms
 * (WLDCORE_TEXT_ID_CALL_BACK_ACCEPTED), and the next press returns to the list. */
void wldcore_proposition_handle_recall_input(wldcore_menu_proposition_confirm_level_t* level) {
    s32 choice;

    if (g_wldcore_window_render_records[level->render_index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (level->confirmed != 0) {
        if (g_wldcore_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS)) {
            wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
            goto close;
        }
        return;
    }
    if (g_wldcore_new_button_presses & PSX_PAD_CROSS) {
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
        choice = level->choice ^ 1;
        level->choice = choice;
        g_wldcore_window_records[level->window].position.y
            = g_wldcore_window_render_records[level->render_index].base.y - -(choice * 0x10 + 0xE);
        wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
    }
    if (!(g_wldcore_new_button_presses & PSX_PAD_CIRCLE)) {
        return;
    }
    if (level->confirmed != 0) {
        return;
    }
    wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
    if (level->choice == 1) {
        goto close;
    }
    world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CALL_BACK_ACCEPTED, 0);
    level->confirmed = 1;
    g_main_active_propositions[level->proposition].flags |= WLDCORE_PROPOSITION_PROGRESS_RECALLED;
    g_wldcore_window_records[level->window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_records[level->second_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
    g_wldcore_window_render_records[level->render_index].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
}
