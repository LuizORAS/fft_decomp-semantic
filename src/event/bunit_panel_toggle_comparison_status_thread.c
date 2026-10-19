#include "fft/event_bunit.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Start (arg=1) or stop (arg=0) sub-thread 7, invoking
 * bunit_panel_unit_status_banner_thread with parameter block g_bunit_comparison_display_thread_params. Sister of
 * thread-8 toggle. */
void bunit_panel_toggle_comparison_status_thread(s32 enable) {
    if (enable != 0) {
        if (thread_is_running(THREAD_SCREEN_SLOT_COMPARISON_STATUS) == 0) {
            thread_start(THREAD_SCREEN_SLOT_COMPARISON_STATUS, bunit_panel_unit_status_banner_thread);
            thread_set_parameters(
                THREAD_SCREEN_SLOT_COMPARISON_STATUS, &g_bunit_comparison_display_thread_params, 0, 0);
        }
    } else {
        bunit_thread_request_stop(THREAD_SCREEN_SLOT_COMPARISON_STATUS);
    }
}
