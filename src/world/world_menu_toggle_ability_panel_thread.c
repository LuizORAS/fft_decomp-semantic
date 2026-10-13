#include "fft/world.h"
#include "psx/types.h"

/* Starts thread 10 on world_menu_ability_panel_thread (parameter g_world_ability_panel_thread_params)
 * when `enable` is set and neither thread 9 nor 10 is running; stops threads 10 and 12 otherwise.
 * Enabling also turns the unit banner on and starts the stats panel (thread 12). */
void world_menu_toggle_ability_panel_thread(s32 enable) {
    s32 state = enable;

    if (state != 0) {
        state = 1;
        if (world_thread_is_running(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL) == 0
            && world_thread_is_running(NATIVE_SCREEN_THREAD_SLOT_CHARACTER_STATUS) == 0) {
            world_thread_start(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL, world_menu_ability_panel_thread);
            world_thread_set_parameters(
                WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL, (s32)&g_world_ability_panel_thread_params, 0, 0);
        }
        g_world_formation_unit_banner_enabled = 1;
        world_menu_toggle_numeric_editor_thread(state);
    } else {
        world_menu_request_thread_stop(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL);
        world_menu_request_thread_stop(NATIVE_SCREEN_THREAD_SLOT_NUMERIC_EDITOR);
    }
}
