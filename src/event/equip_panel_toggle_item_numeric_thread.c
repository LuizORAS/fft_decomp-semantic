#include "fft/event_equip.h"
#include "psx/types.h"

void equip_panel_toggle_item_numeric_thread(s32 enable) {
    if (enable != 0) {
        if (battle_thread_is_running(EQUIP_THREAD_SLOT_ITEM_NUMERIC) == 0) {
            battle_thread_start(EQUIP_THREAD_SLOT_ITEM_NUMERIC, equip_editor_run_numeric_thread);
            battle_thread_set_parameters(EQUIP_THREAD_SLOT_ITEM_NUMERIC, (s32)&g_equip_item_numeric_panel_params, 0, 0);
        }
    } else {
        equip_thread_request_stop(EQUIP_THREAD_SLOT_ITEM_NUMERIC);
    }
    g_equip_item_numeric_thread_enabled = enable;
    g_equip_numeric_panel_style = enable;
}
