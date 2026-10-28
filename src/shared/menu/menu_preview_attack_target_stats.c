#include "fft/battle_action.h"
#include "fft/battle_menu.h"
#include "fft/menu.h"
#include "fft/thread.h"

/* Attack-preview target panel (the unit view's hovered-unit slot). WORLD compiles this text too.
 *
 * It waits for the right-side panel thread to end unless that thread shows the attack caster, loads the
 * preview target's billboard by swapping it in as the active-turn unit for one call, carries two fields
 * of that billboard into the comparison copy, reloads the real unit and runs the stats panel. */
void menu_preview_attack_target_stats(void) {
    s32 saved;

    if (g_thread_contexts[THREAD_SLOT_RIGHT_UNIT_STATS].task_id != THREAD_TASK_ATTACK_CASTER_STATS) {
        thread_wait_until_inactive(THREAD_SLOT_RIGHT_UNIT_STATS);
    }
    thread_yield();
    thread_yield();
    saved = g_menu_selected_unit.battle_id;
    /* The target loads the preview unit id unsigned (lhu). */
    g_menu_selected_unit.battle_id = g_preview_target_unit_id;
    battle_menu_load_selected_unit();
    g_menu_selected_unit.battle_id = saved;
    g_menu_comparison_billboard_data.state.generic_monster = g_menu_billboard_data.state.generic_monster;
    g_menu_comparison_billboard_data.state.ability_ids[2] = g_menu_billboard_data.state.ability_ids[2];
    battle_menu_load_selected_unit();
    battle_menu_display_hovered_unit_stats(&g_menu_attack_target_stats_rect, UNIT_STATS_PANEL_ATTACK_TARGET,
        g_battle_menu_attack_target_stats_setup, &g_menu_comparison_unit_status);
}
