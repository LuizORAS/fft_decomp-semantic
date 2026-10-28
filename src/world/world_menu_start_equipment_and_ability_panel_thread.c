#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* When `enabled` is set (the formation passes its browse flag) and the status banner is not sliding,
 * start the equipment-and-ability panel (world_menu_equipment_and_ability_panel_thread in slot 9 with
 * g_world_equipment_ability_panel_thread_params) unless slot 9 or 10 runs, and the stats panel
 * (thread 12). */
void world_menu_start_equipment_and_ability_panel_thread(s32 enabled) {
    if (enabled == 0) {
        return;
    }
    enabled = 1;
    if (g_world_status_display_thread_params.flags & (MENU_STATUS_PANEL_SLIDE_DOWN | MENU_STATUS_PANEL_SLIDE_UP)) {
        return;
    }
    if (thread_is_running(THREAD_SCREEN_SLOT_CHARACTER_STATUS) == 0
        && thread_is_running(WORLD_THREAD_SLOT_EQUIP_ABILITY_PANEL) == 0) {
        thread_start(THREAD_SCREEN_SLOT_CHARACTER_STATUS, world_menu_equipment_and_ability_panel_thread);
        thread_set_parameters(
            THREAD_SCREEN_SLOT_CHARACTER_STATUS, (s32)&g_world_equipment_ability_panel_thread_params, 0, 0);
    }
    world_menu_toggle_numeric_editor_thread(enabled);
}
