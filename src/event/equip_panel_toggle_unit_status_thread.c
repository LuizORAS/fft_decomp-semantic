#include "fft/event_equip.h"
#include "psx/types.h"

/* Starts the unit status thread on equip_render_unit_status_panel_thread
 * (parameter g_equip_unit_status_panel_params) when `enable` is set and the
 * thread is idle; stops it otherwise. */
void equip_panel_toggle_unit_status_thread(s32 enable) {
    if (enable != 0) {
        if (battle_thread_is_running(EQUIP_THREAD_SLOT_UNIT_STATUS) == 0) {
            battle_thread_start(EQUIP_THREAD_SLOT_UNIT_STATUS, equip_render_unit_status_panel_thread);
            battle_thread_set_parameters(EQUIP_THREAD_SLOT_UNIT_STATUS, (s32)g_equip_unit_status_panel_params, 0, 0);
        }
    } else {
        equip_thread_request_stop(EQUIP_THREAD_SLOT_UNIT_STATUS);
    }
}
