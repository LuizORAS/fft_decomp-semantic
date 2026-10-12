#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "fft/battle_script.h"
#include "psx/pad.h"
#include "psx/types.h"

/* Act on a confirmed menu option when circle is pressed.
 *
 * An action in 0..0xff starts that idle-action menu entry's thread on the
 * thread below the current one (bit 0x1000 suppresses the confirm sound);
 * 0x4d2 records the option as -1; a negative action -n closes n menu levels
 * by setting the third parameter of the next n threads (thread 8 also saves
 * the open menus' selections). */
void battle_menu_handle_action(menu_entry_t* menu, s32 selected_option) {
    menu_entry_t* entry;
    s16* actions;
    s32 action;
    s32 silent;
    s32 i;
    s32 j;
    s32 thread_id;

    entry = menu;
    if (g_battle_menu_input_mode == MENU_INPUT_TUTORIAL
        && ((script_get_variable(EVENT_SCRIPT_VAR_DISABLED_MENU_ACTIONS) >> selected_option) & 1)) {
        return;
    }
    silent = 0;
    if (!(g_script_event_input & PSX_PAD_CIRCLE) || battle_menu_can_accept_input() == 0) {
        return;
    }
    if (g_battle_thread_contexts[g_battle_current_thread_id].task_words[1] == 0) {
        g_battle_thread_contexts[g_battle_current_thread_id].task_words[1] = 2;
        g_battle_thread_contexts[g_battle_current_thread_id].task_words[0] = 0;
    }
    actions = entry->row_actions;
    if ((s32)actions >= -15 && (s32)actions <= -1) {
        action = (s32)actions;
    } else {
        action = actions[selected_option];
    }
    if (action >= 0) {
        if (action & MENU_ROW_ACTION_SILENT) {
            silent = 1;
            /* Zero-instruction use: raises silent's allocation priority so it
               takes $s6 and selected_option keeps $s7, as in the target. */
            __asm__("" : : "r"(silent), "r"(silent));
            action -= MENU_ROW_ACTION_SILENT;
        }
    }
    if (action >= 0) {
        if (action < 0x100) {
            battle_thread_start(g_battle_current_thread_id - 1, g_battle_menu_thread_menu_data[action].thread_entry);
            battle_thread_set_parameters(
                g_battle_current_thread_id - 1, (s32)&g_battle_menu_thread_menu_data[action], 0, 0);
        } else if (action == MENU_ROW_ACTION_CLEAR_SELECTION) {
            selected_option = -1;
        }
    } else {
        action = -action;
        for (i = 0; i < 16; i++) {
            if (g_battle_thread_contexts[i].task_id == NATIVE_THREAD_TASK_RESUME) {
                g_battle_thread_contexts[i].task_id = NATIVE_THREAD_TASK_STOP_REQUEST;
            }
        }
        for (i = 0; i < action; i++) {
            thread_id = i + g_battle_current_thread_id;
            g_battle_thread_contexts[thread_id].function_parameter_3 = 1;
            if (thread_id == 8) {
                for (j = 0; j < action; j++) {
                    if (battle_thread_is_running_asm(8 - j) != 0) {
                        g_battle_menu_pending_selection[j]
                            = ((menu_entry_t*)g_battle_thread_contexts[8 - j].function_parameter_1)->selected_index;
                    }
                }
            }
        }
    }
    g_script_event_input = 0;
    *menu_entry_result(entry) = selected_option;
    if (g_sound_suppressed == 0 && silent == 0) {
        sound_set_effect_to_confirm();
    }
}
