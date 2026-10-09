#include "fft/event_require.h"
#include "psx/types.h"

void require_overlay_open_jobstts(void) {
    void* allocation;

    battle_menu_copy_billboard_data_in(
        &g_require_panel_selected_billboard, &g_require_active_unit_data, &g_require_editor_unit_fields);
    while (1) {
        allocation = main_heap_reserve_at(g_main_heap_high_overlay_load_address, 0x1f000);
        if (allocation == g_main_heap_high_overlay_load_address) {
            break;
        }
        battle_thread_yield();
        if (g_main_font_print_enabled != 0) {
            FntPrint(g_require_text_wait_for_file_allocation_message);
        }
    }
    battle_menu_request_open_companion_executable(BATTLE_COMPANION_OVERLAY_JOBSTTS);
    battle_thread_wait_frames(2);
    g_battle_companion_overlay_state = BATTLE_COMPANION_OVERLAY_STATE_JOBSTTS;
    do {
        g_require_input_controller = battle_script_get_controller_input_pointer(0);
        *g_require_input_controller = 0;
        battle_thread_yield();
    } while (g_battle_companion_overlay_state != BATTLE_COMPANION_OVERLAY_STATE_IDLE);
    *g_require_input_controller = 0;
    main_heap_free(allocation);
    battle_thread_wait_frames(2);
    g_option_menu_open = 0;
    battle_thread_exit_current();
}
