#include "fft/world.h"
#include "psx/types.h"

/* Start the equipment panel (world_menu_equipment_panel_thread in slot 10 with
 * g_world_change_banner_panel_thread_params) and the stats panel (thread 12) when `enable` is set and
 * the status banner is not sliding; stop both otherwise. */
void world_menu_toggle_change_banner_panel_thread(s32 enable) {
    s32 state = enable;

    if (state != 0) {
        state = 1;
        if (!(g_world_status_display_thread_params.flags
                & (MENU_STATUS_PANEL_SLIDE_DOWN | MENU_STATUS_PANEL_SLIDE_UP))) {
            if (world_thread_is_running(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL) == 0) {
                world_thread_start(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL, world_menu_equipment_panel_thread);
                world_thread_set_parameters(
                    WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL, (s32)&g_world_change_banner_panel_thread_params, 0, 0);
            }
            world_menu_toggle_numeric_editor_thread(state);
        }
    } else {
        world_menu_request_thread_stop(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL);
        world_menu_request_thread_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    }
}
