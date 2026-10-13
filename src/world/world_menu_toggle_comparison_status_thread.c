#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Starts thread 7 on world_menu_unit_status_banner_thread (parameter g_world_comparison_display_thread_params) when
 * `flag` is set and the thread is idle; stops it otherwise. */
void world_menu_toggle_comparison_status_thread(s32 flag) {
    if (flag != 0) {
        if (world_thread_is_running(THREAD_SCREEN_SLOT_COMPARISON_STATUS) == 0) {
            thread_start(THREAD_SCREEN_SLOT_COMPARISON_STATUS, world_menu_unit_status_banner_thread);
            thread_set_parameters(
                THREAD_SCREEN_SLOT_COMPARISON_STATUS, (s32)&g_world_comparison_display_thread_params, 0, 0);
        }
    } else {
        world_menu_stop_unit_status_banner_thread(THREAD_SCREEN_SLOT_COMPARISON_STATUS);
    }
}
