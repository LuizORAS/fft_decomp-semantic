#include "fft/world.h"
#include "psx/types.h"

/* Starts thread 12 on world_menu_run_numeric_editor_thread (parameter
 * g_world_numeric_editor_thread_params) when `enable` is set and the thread
 * is idle; stops it otherwise. */
void world_menu_toggle_numeric_editor_thread(s32 enable) {
    if (enable != 0) {
        if (world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR) == 0) {
            world_thread_start(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR, world_menu_run_numeric_editor_thread);
            world_thread_set_parameters(
                NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR, (s32)&g_world_numeric_editor_thread_params, 0, 0);
        }
    } else {
        world_menu_request_thread_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    }
}
