#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "fft/battle_text.h"
#include "fft/battle_unit.h"
#include "fft/thread.h"
#include "psx/types.h"

/* Requests a unit view mode from the unit view supervisor thread, starting the
 * thread when it is not running.
 *
 * The unit name list is rebuilt and the panel fade mode taken from
 * g_battle_menu_panel_fade_modes[mode]. unit_a becomes the active-turn unit of
 * the banner and unit_b the previewed target of the status billboard; 0xff leaves
 * either unchanged. Ignored while g_battle_menu_suspend_state is 1. */
void battle_unit_start_view_thread(s32 mode, s32 unit_a, s32 unit_b) {
    if (g_battle_menu_suspend_state == 1) {
        return;
    }
    battle_text_build_unit_name_list();
    g_battle_menu_panel_fade_mode = g_battle_menu_panel_fade_modes[mode];
    if (unit_a != 0xff) {
        g_battle_active_turn_unit.battle_id = unit_a;
        g_battle_menu_active_turn_banner.unit_index = unit_a;
    }
    if (unit_b != 0xff) {
        g_battle_preview_target_unit_id = unit_b;
        g_battle_menu_status_billboard.unit_index = unit_b;
    }
    if (battle_thread_is_running_asm(THREAD_SLOT_UNIT_VIEW) == 0) {
        thread_start(THREAD_SLOT_UNIT_VIEW, battle_unit_view_supervisor_thread);
    }
    thread_set_parameters(THREAD_SLOT_UNIT_VIEW, mode, 0, 0);
}
