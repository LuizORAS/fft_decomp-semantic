#include "fft/event_card.h"
#include "psx/pad.h"
#include "psx/types.h"

void card_save_update_slot_scan_result(void) {
    s32 index;
    s32 all_empty;

    if (card_io_poll_operation_result() > CARD_IO_RESULT_COMPLETE) {
        g_card_save_slot_scan_prompt_descriptor = 0;
        g_main_card_save_last_used_slot = CARD_SAVE_LAST_USED_SLOT_NONE;
        card_menu_stop_thread_and_wait(14);
        card_menu_stop_thread_and_wait(NATIVE_SCREEN_THREAD_SLOT_MENU);
        g_card_save_menu_state = CARD_SAVE_MENU_SCANNING;
    } else if (g_card_save_slot_scan_prompt_descriptor != 0) {
        card_menu_start_or_poll_thread(NATIVE_SCREEN_THREAD_SLOT_MENU, g_card_save_slot_scan_prompt_descriptor);
        if ((g_card_input_primary_repeat & PSX_PAD_CROSS) != 0 || (g_card_input_primary_repeat & PSX_PAD_CIRCLE) != 0) {
            g_card_save_slot_scan_prompt_descriptor = 0;
            g_main_card_save_last_used_slot = CARD_SAVE_LAST_USED_SLOT_NONE;
            card_menu_stop_thread_and_wait(NATIVE_SCREEN_THREAD_SLOT_MENU);
            g_card_save_menu_state = CARD_SAVE_MENU_SELECT_CARD;
        }
    } else {
        index = 0;
        all_empty = 1;
        while (index < 15) {
            if (g_card_save_slot_file_states[index] != CARD_SAVE_SLOT_UNAVAILABLE) {
                all_empty = 0;
            }
            index++;
        }

        if (all_empty != 0) {
            g_card_save_slot_scan_prompt_descriptor = g_card_save_no_data_prompt_descriptor;
        }
        if (g_card_save_slot_scan_prompt_descriptor == 0) {
            g_card_save_list_scroll_base_y = 24;
            g_card_save_list_scroll_anim_offset = 0;
            g_card_save_list_top_row = 0;
            g_card_save_list_selected_slot = 0;
            g_card_save_list_cursor_index = 0;
            g_card_save_occupied_slot_count = card_save_init_occupied_slot_selection();
            g_card_save_menu_state = CARD_SAVE_MENU_SELECT_SAVE;
        } else {
            g_card_sound_queued_effect_id = MAIN_SFX_ERROR_MESSAGE;
        }
    }
}
