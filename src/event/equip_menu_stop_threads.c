#include "fft/event_equip.h"
#include "psx/types.h"

/* Ask the unit status, equipment list, numeric editor and item numeric
 * panel threads to stop and clear the two panel styles. */
void equip_menu_stop_threads(void) {
    equip_thread_request_stop(EQUIP_THREAD_SLOT_UNIT_STATUS);
    equip_thread_request_stop(EQUIP_THREAD_SLOT_EQUIPMENT_LIST);
    equip_thread_request_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    equip_panel_toggle_item_numeric_thread(0);
    g_equip_item_numeric_panel_style = 0;
    g_equip_equipment_panel_style = 0;
}
