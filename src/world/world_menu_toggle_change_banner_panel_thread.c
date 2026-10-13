#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Starts thread 10 on world_menu_equipment_panel_thread (parameter g_world_change_banner_panel_thread_params) when
 * `enable` is set and neither of the g_world_status_display_thread_params.flags 0x60 bits is set; stops threads 10 and
 * 12 otherwise.
 */
void world_menu_toggle_change_banner_panel_thread(s32 enable) {
    s32 state = enable;

    if (state != 0) {
        state = 1;
        if (!(g_world_status_display_thread_params.flags
                & (MENU_STATUS_PANEL_SLIDE_DOWN | MENU_STATUS_PANEL_SLIDE_UP))) {
            if (world_thread_is_running(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL) == 0) {
                thread_start(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL, world_menu_equipment_panel_thread);
                thread_set_parameters(
                    WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL, (s32)&g_world_change_banner_panel_thread_params, 0, 0);
            }
            world_menu_toggle_numeric_editor_thread(state);
        }
    } else {
        world_menu_stop_unit_status_banner_thread(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL);
        world_menu_stop_unit_status_banner_thread(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    }
}
