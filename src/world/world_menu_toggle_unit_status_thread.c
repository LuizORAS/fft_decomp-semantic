#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Starts thread 8 on world_menu_unit_status_banner_thread (parameter
 * g_world_status_display_thread_params) when `enable` is set and the thread
 * is idle; stops it otherwise. */
void world_menu_toggle_unit_status_thread(s32 enable) {
    if (enable != 0) {
        if (thread_is_running(THREAD_SCREEN_SLOT_UNIT_STATUS) == 0) {
            thread_start(THREAD_SCREEN_SLOT_UNIT_STATUS, world_menu_unit_status_banner_thread);
            thread_set_parameters(THREAD_SCREEN_SLOT_UNIT_STATUS, (s32)&g_world_status_display_thread_params, 0, 0);
        }
    } else {
        world_menu_request_thread_stop(THREAD_SCREEN_SLOT_UNIT_STATUS);
    }
}
