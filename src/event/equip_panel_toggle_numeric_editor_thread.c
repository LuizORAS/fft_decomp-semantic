#include "fft/event_equip.h"
#include "psx/types.h"

/* Starts thread 12 on equip_editor_run_numeric_thread (parameter
 * g_equip_numeric_panel_params) when `enable` is set and the thread is idle;
 * stops it otherwise. */
void equip_panel_toggle_numeric_editor_thread(s32 enable) {
    if (enable != 0) {
        if (battle_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) == 0) {
            battle_thread_start(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR, equip_editor_run_numeric_thread);
            battle_thread_set_parameters(
                NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR, (s32)g_equip_numeric_panel_params, 0, 0);
        }
    } else {
        equip_thread_request_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    }
}
