#include "fft/wldcore.h"
#include "psx/pad.h"

/* Input step of the status help: the d-pad moves the cursor to the neighbouring field
 * (g_wldcore_status_help_fields), Circle shows the field's help text (its base text id, plus the
 * field's current value when it has one; a value of -1 or 0 shows nothing, except on field 0x0b) and
 * Cross closes the level. */
void wldcore_menu_handle_status_help_input(wldcore_menu_status_help_level_t* request) {
    s32 field;
    s16* value;
    s16 message;
    wldcore_window_record_t* record;
    s32 window;

    if ((g_wldcore_new_button_presses & PSX_PAD_CROSS) != 0) {
        g_wldcore_window_record_count -= 1;
        g_wldcore_window_render_object_count -= 1;
        g_wldcore_menu_ordering_table_offset = request->saved_state;
        g_wldcore_menu_stack_depth -= 1;
        wldcore_menu_dispatch_resume_handler();
        wldcore_sound_play_effect(MAIN_SFX_CANCEL);
        return;
    }

    if ((g_wldcore_new_button_presses & PSX_PAD_CIRCLE) != 0) {
        field = request->field;
        value = g_wldcore_status_help_fields[field].value;
        if (value != 0) {
            if (field != 0xB && *value >= -1 && *value < 1) {
                return;
            }
            message = g_wldcore_status_help_fields[field].message_base + *value;
        } else {
            message = g_wldcore_status_help_fields[field].message_base;
        }
        wldcore_menu_push_message_level(message, 1);
        g_world_menu_input_lock_mode = 1;
        window = request->window_index;
        record = &g_wldcore_window_records[window];
        record->sequence = g_wldcore_window_records[window].sequence - 1;
        g_wldcore_window_records[window].palette = 0xA;
        record->anim_counter = 0;
        g_wldcore_window_records[window].frame_index = 0;
        return;
    }

    if (wldcore_input_check_repeating_directional(PSX_PAD_UP) != 0) {
        if (g_wldcore_status_help_fields[request->field].up != request->field) {
            wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
            request->field = g_wldcore_status_help_fields[request->field].up;
            wldcore_window_move_status_help_cursor(request);
            return;
        }
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_DOWN) != 0) {
        if (g_wldcore_status_help_fields[request->field].down != request->field) {
            wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
            request->field = g_wldcore_status_help_fields[request->field].down;
            wldcore_window_move_status_help_cursor(request);
            return;
        }
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_RIGHT) != 0) {
        if (g_wldcore_status_help_fields[request->field].right != request->field) {
            wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
            request->field = g_wldcore_status_help_fields[request->field].right;
            wldcore_window_move_status_help_cursor(request);
            return;
        }
    }
    if (wldcore_input_check_repeating_directional(PSX_PAD_LEFT) != 0) {
        if (g_wldcore_status_help_fields[request->field].left != request->field) {
            wldcore_sound_play_effect(MAIN_SFX_CURSOR_MOVE);
            request->field = g_wldcore_status_help_fields[request->field].left;
            wldcore_window_move_status_help_cursor(request);
        }
    }
}
