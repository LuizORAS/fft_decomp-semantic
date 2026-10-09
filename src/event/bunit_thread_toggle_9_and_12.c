#include "fft/event_bunit.h"
#include "psx/types.h"

/* Start (enable=1) or stop (enable=0) sub-threads 9 and 12. Sister of
 * bunit_panel_toggle_comparison_status_thread / _8. */
void bunit_thread_toggle_9_and_12(s32 enable) {
    if (enable != 0) {
        if (g_bunit_status_display_flags & (MENU_STATUS_PANEL_SLIDE_DOWN | MENU_STATUS_PANEL_SLIDE_UP)) {
            return;
        }
        if (battle_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_CHARACTER_STATUS) != 0) {
            return;
        }
        battle_thread_start(NATIVE_SCREEN_THREAD_SLOT_CHARACTER_STATUS, bunit_menu_equipment_and_ability_panel_thread);
        battle_thread_set_parameters(
            NATIVE_SCREEN_THREAD_SLOT_CHARACTER_STATUS, &g_bunit_character_status_frame_config, 0, 0);
        battle_thread_start(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR, bunit_menu_run_numeric_editor_thread);
        battle_thread_set_parameters(
            NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR, &g_bunit_numeric_editor_thread_params, 0, 0);
    } else {
        bunit_thread_request_stop(NATIVE_SCREEN_THREAD_SLOT_CHARACTER_STATUS);
        bunit_thread_request_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    }
}
