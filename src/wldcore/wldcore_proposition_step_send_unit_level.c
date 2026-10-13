#include "fft/wldcore.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Step of the send-unit level. In WLDCORE_SEND_UNIT_MODE_SELECT the highlighted unit is previewed
 * and Circle adds it to the three slots or takes it out (a fourth goes to
 * wldcore_proposition_step_send_unit_slot_input to replace one), Square opens its status
 * (wldcore_proposition_step_send_unit_status), Select the help text, Start sends the chosen units (at
 * least one) and Cross gives up. Sending closes the level and asks for the number of days
 * (wldcore_proposition_push_days_level) over the proposition's detail; giving up returns to the list
 * on offer.
 *
 * The target reserves a 0x30-byte frame but only saves s0 and ra; the unused
 * 16-byte array reproduces that frame. The removal and addition paths each
 * end in their own redraw calls, which jump2 cross-jumps from the count
 * store onward. */
void wldcore_proposition_step_send_unit_level(wldcore_menu_send_unit_level_t* level) {
    s32 buttons;
    s32 mode;
    s32 value;
    s32 i;
    s32 unused[4];

    mode = level->mode;
    if (mode == WLDCORE_SEND_UNIT_MODE_CANCEL) {
        if (world_thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
            g_wldcore_menu_blocked_buttons = 0;
            g_main_system_flags &= ~MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            g_wldcore_menu_stack_depth--;
            wldcore_proposition_open_available_list();
        }
        return;
    }
    if (mode == WLDCORE_SEND_UNIT_MODE_ACCEPT) {
        if (world_thread_is_running(WLDCORE_THREAD_SLOT_WINDOW_PANEL) == 0) {
            g_wldcore_menu_blocked_buttons = 0;
            g_main_system_flags &= ~MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
            g_wldcore_menu_stack_depth--;
            wldcore_proposition_push_detail_level(g_wldcore_selected_proposition_row[0].fields.id - 1);
            wldcore_menu_suspend_proposition_detail_level((s32*)level);
            wldcore_proposition_push_days_level();
        }
        return;
    }
    if ((mode - 3) < 2U || mode == WLDCORE_SEND_UNIT_MODE_STATUS_CLOSE) {
        wldcore_proposition_step_send_unit_status(level);
        return;
    }
    if (mode == WLDCORE_SEND_UNIT_MODE_REPLACE) {
        wldcore_proposition_step_send_unit_slot_input(level);
        return;
    }

    if (!(g_wldcore_window_render_records[level->layout_render].flags & WLDCORE_DISPLAY_OBJECT_POP_IN)) {
        value = g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index] & TEXT_ID_ENTRY_MASK;
        if (value != level->shown_preview) {
            level->shown_preview = value;
            wldcore_window_load_party_unit_sprite_record(level, value, level->preview_render);
        }
    }

    buttons = g_wldcore_new_button_presses;
    if (buttons & PSX_PAD_SELECT) {
        wldcore_menu_suspend_proposition_send_unit_level((wldcore_window_tint_request_t*)level);
        wldcore_menu_push_message_level(0x109A, 1);
        return;
    }
    if (buttons & PSX_PAD_START) {
        if (g_wldcore_proposition_send_unit_count == 0) {
            wldcore_sound_play_effect(MAIN_SFX_INVALID);
            return;
        }
        wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
        level->mode = WLDCORE_SEND_UNIT_MODE_ACCEPT;
        g_wldcore_context_value_display_mode = WLDCORE_VALUE_DISPLAY_COST_AND_FUNDS;
        g_wldcore_displayed_numeric_value_secondary = g_wldcore_proposition_gil_amount;
        /* Shared tail: a duplicated copy is scheduled differently and
         * only partly cross-jumped. */
        goto suspend;
    }
    if (buttons & PSX_PAD_CROSS) {
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        level->mode = WLDCORE_SEND_UNIT_MODE_CANCEL;
    suspend:
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        g_wldcore_window_record_count -= 7;
        g_wldcore_window_render_record_count -= 5;
        g_wldcore_window_render_object_count -= 0xC;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_INPUT_BLOCKED;
        return;
    }
    if (buttons & PSX_PAD_SQUARE) {
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_WINDOW_PANEL, 0, 0, 1);
        world_thread_set_parameters(WLDCORE_THREAD_SLOT_MESSAGE, 0, -1, 0);
        g_wldcore_window_records[level->list_window].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[level->layout_render].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->frame_window_a].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->frame_window_b].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[level->preview_render].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_render_records[level->list_render].flags |= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_main_system_flags |= MAIN_SYSTEM_FLAG_HIDE_CONTEXT_VALUE;
        level->mode = WLDCORE_SEND_UNIT_MODE_STATUS_OPEN;
        return;
    }
    if (!(buttons & PSX_PAD_CIRCLE)) {
        return;
    }

    wldcore_sound_play_effect(MAIN_SFX_CONFIRM);
    value = g_wldcore_list_entry_values[g_wldcore_window_panel_render_state.selected_index] & TEXT_ID_ENTRY_MASK;
    for (i = 0; i < g_wldcore_proposition_send_unit_count; i++) {
        if (g_wldcore_proposition_send_units[i] == value) {
            for (value = i + 1; value < 3; value++) {
                g_wldcore_proposition_send_units[value - 1] = g_wldcore_proposition_send_units[value];
            }
            g_wldcore_proposition_send_unit_count--;
            wldcore_window_draw_text_rows(level);
            wldcore_proposition_load_send_unit_candidates(level);
            world_thread_request_redraw(WLDCORE_THREAD_SLOT_WINDOW_PANEL);
            return;
        }
    }

    if (g_wldcore_proposition_send_unit_count == 3) {
        g_wldcore_window_records[level->frame_window_a].palette
            = g_wldcore_window_records[level->frame_window_b].palette = 0xA;
        g_wldcore_window_render_records[level->portrait_frame].flags |= WLDCORE_DISPLAY_OBJECT_POP_IN;
        g_wldcore_window_render_records[level->portrait_frame].flags ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_menu_blocked_buttons = -1;
        g_wldcore_window_records[level->portrait_window].flags ^= WLDCORE_DISPLAY_OBJECT_HIDDEN;
        level->shown_unit = -1;
        level->mode = WLDCORE_SEND_UNIT_MODE_REPLACE;
        level->slot = 0;
        g_wldcore_window_records[level->cursor_window].flags &= ~WLDCORE_DISPLAY_OBJECT_HIDDEN;
        g_wldcore_window_records[level->cursor_window].position.x
            = g_wldcore_window_render_records[level->layout_render].base.x + 6;
        g_wldcore_window_records[level->cursor_window].position.y
            = g_wldcore_window_render_records[level->layout_render].base.y + level->slot * 16 + 0xC;
        return;
    }
    g_wldcore_proposition_send_units[g_wldcore_proposition_send_unit_count] = value;
    g_wldcore_proposition_send_unit_count++;

    wldcore_window_draw_text_rows(level);
    wldcore_proposition_load_send_unit_candidates(level);
    world_thread_request_redraw(WLDCORE_THREAD_SLOT_WINDOW_PANEL);
}
