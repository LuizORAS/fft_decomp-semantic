#include "fft/event_bunit.h"
#include "psx/types.h"

/* Start (arg=1) or stop (arg=0) sub-thread 8, invoking
 * bunit_panel_unit_status_banner_thread with parameter block g_bunit_status_display_thread_params. */
void bunit_panel_toggle_unit_status_thread(s32 enable) {
    if (enable != 0) {
        if (battle_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS) == 0) {
            battle_thread_start(NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS, bunit_panel_unit_status_banner_thread);
            battle_thread_set_parameters(
                NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS, &g_bunit_status_display_thread_params, 0, 0);
        }
    } else {
        bunit_thread_request_stop(NATIVE_SCREEN_THREAD_SLOT_UNIT_STATUS);
    }
}
