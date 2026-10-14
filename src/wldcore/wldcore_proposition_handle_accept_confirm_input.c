#include "fft/wldcore.h"
#include "psx/pad.h"

/* Step of the take-the-job yes/no panel. Up and down switch the row; Cross and "no" return to the
 * list on offer. "Yes" needs a unit to send (a generic human of the first 16 roster slots who is not
 * away on a proposition, the filter of wldcore_proposition_load_send_unit_candidates) and the war funds
 * for the fee: without a unit the barkeeper says so twice (WLDCORE_TEXT_ID_CHOOSE_MORE_UNITS, then
 * WLDCORE_TEXT_ID_CHOOSE_HUMAN_UNIT), without the gil once (WLDCORE_TEXT_ID_NOT_ENOUGH_GIL), and the
 * next press returns to the list; otherwise the units are chosen
 * (wldcore_proposition_push_send_unit_level).
 *
 * The gotos are the target's shared tails: the mode 3 close, the "no"
 * dismissal and the no-unit failure jump into the cancel and gil-failure
 * blocks; duplicated tails do not cross-jump back into that shape. */
void wldcore_proposition_handle_accept_confirm_input(wldcore_menu_choice_panel_level_t* level) {
    s32 available;
    s32 i;
    s32 row;
    party_data_t* unit;

    if (g_wldcore_window_render_records[level->render_index].flags & WLDCORE_DISPLAY_OBJECT_POP_IN) {
        return;
    }
    if (level->mode == WLDCORE_ACCEPT_CONFIRM_MODE_CLOSE_MESSAGE) {
        if (!(g_wldcore_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS))) {
            return;
        }
        goto close;
    }
    if (level->mode == WLDCORE_ACCEPT_CONFIRM_MODE_NO_UNIT_MESSAGE) {
        if (g_wldcore_new_button_presses & (PSX_PAD_CIRCLE | PSX_PAD_CROSS)) {
            world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CHOOSE_HUMAN_UNIT, 0);
            level->mode = WLDCORE_ACCEPT_CONFIRM_MODE_CLOSE_MESSAGE;
        }
        return;
    }
    if (g_wldcore_new_button_presses & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
    dismiss:
        g_wldcore_window_record_count -= 2;
        g_wldcore_window_render_record_count--;
        g_wldcore_window_render_object_count -= 3;
    close:
        g_wldcore_window_render_record_count--;
        g_wldcore_window_render_object_count--;
        g_wldcore_menu_stack_depth -= 2;
        wldcore_proposition_open_available_list();
        return;
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_UP) != 0
        || wldcore_input_check_repeating_directional(PSX_PAD_DOWN) != 0) {
        row = level->row ^ 1;
        level->row = row;
        g_wldcore_window_records[level->cursor_window].position.y
            = g_wldcore_window_render_records[level->render_index].base.y - -(row * 0x10 + 0xE);
        wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
    }
    if (!(g_wldcore_new_button_presses & PSX_PAD_CIRCLE)) {
        return;
    }
    if (level->row == 1) {
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        goto dismiss;
    }
    available = 0;
    for (i = 0; i < PARTY_GUEST_SLOT_FIRST; i++) {
        unit = wldcore_unit_get_party_data_pointer(i);
        if (unit->proposition_status == 0 && unit->party_id != PARTY_ID_NONE
            && !(unit->gender_flags & UNIT_FLAG_MONSTER) && unit->sprite_set >= CHARACTER_IDENTITY_BOY) {
            available++;
        }
    }
    if (available == 0) {
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_CHOOSE_MORE_UNITS, 0);
        level->mode = WLDCORE_ACCEPT_CONFIRM_MODE_NO_UNIT_MESSAGE;
        goto fail;
    }
    if (world_script_get_variable(EVENT_SCRIPT_VAR_WAR_FUNDS) < g_wldcore_proposition_gil_amount) {
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0x19, WLDCORE_TEXT_ID_NOT_ENOUGH_GIL, 0);
        level->mode = WLDCORE_ACCEPT_CONFIRM_MODE_CLOSE_MESSAGE;
    fail:
        wldcore_sound_play_effect(MAIN_SFX_INVALID);
        g_wldcore_window_record_count -= 2;
        g_wldcore_window_render_record_count--;
        g_wldcore_window_render_object_count -= 3;
        return;
    }
    wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
    g_wldcore_window_record_count -= 2;
    g_wldcore_window_render_record_count--;
    g_wldcore_window_render_object_count -= 3;
    g_wldcore_window_render_record_count--;
    g_wldcore_window_render_object_count--;
    g_wldcore_menu_stack_depth -= 2;
    wldcore_proposition_push_send_unit_level();
}
