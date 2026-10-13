#include "fft/thread.h"
#include "fft/world.h"
#include "psx/types.h"

/* Start (or re-parameterise) the unit view supervisor thread in `mode`,
 * selecting `unit_a` as the active unit and `unit_b` as the comparison unit;
 * 0xFF keeps the current one. Does nothing while a menu overlay is open. */
void world_unit_start_view_thread(s32 mode, s32 unit_a, s32 unit_b) {
    if (g_world_menu_overlay_state != 1) {
        world_text_build_unit_name_list();
        g_world_menu_panel_fade_mode = g_world_unit_view_panel_fade_modes[mode];
        if (unit_a != 0xFF) {
            g_world_active_unit_data.battle_id = unit_a;
            g_world_unit_selected_status_billboard.unit_index = unit_a;
        }
        if (unit_b != 0xFF) {
            g_world_unit_comparison_battle_id = unit_b;
            g_world_unit_comparison_status_billboard.unit_index = unit_b;
        }
        if (world_thread_is_running_asm(THREAD_SLOT_UNIT_VIEW) == 0) {
            thread_start(THREAD_SLOT_UNIT_VIEW, world_unit_view_supervisor_thread);
        }
        thread_set_parameters(THREAD_SLOT_UNIT_VIEW, mode, 0, 0);
    }
}
