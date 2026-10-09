#include "fft/battle.h"
#include "psx/types.h"

/* Non-blocking two-step file load driven by g_battle_thread_call_target:
 * issue the read for the pending request, then poll until it completes and
 * clear the request. */
void battle_menu_open_companion_data_file(void) {
    if ((g_battle_menu_companion_data_file_request_index != 0) && (g_battle_menu_companion_data_file_loading == 0)) {
        g_battle_thread_call_target = (void (*)(void))main_file_call_build_header;
        if (battle_thread_call_on_main_stack(
                g_battle_menu_companion_data_file_lbas[g_battle_menu_companion_data_file_request_index],
                g_battle_menu_companion_data_file_sizes[g_battle_menu_companion_data_file_request_index],
                (s32)g_battle_text_message_buffer)
            == 0) {
            g_battle_menu_companion_data_file_loading = 1;
        }
    } else if ((g_battle_menu_companion_data_file_request_index != 0)
        && (g_battle_menu_companion_data_file_loading != 0)) {
        g_battle_thread_call_target = main_file_is_still_loading;
        if (battle_thread_call_on_main_stack() == 0) {
            g_battle_menu_companion_data_file_loading = 0;
            g_battle_menu_companion_data_file_request_index = 0;
        }
    }
}
