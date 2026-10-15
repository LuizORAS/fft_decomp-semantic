#include "fft/battle.h"
#include "psx/types.h"

/* Attack-preview target panel (the unit view's hovered-unit slot).
 *
 * It waits for the right-side panel thread to end unless that thread shows the attack caster, loads the
 * preview target's billboard by swapping it in as the active-turn unit for one call, carries two fields
 * of that billboard into the comparison copy, reloads the real unit and runs the stats panel. */
void battle_menu_preview_attack_target_stats(void) {
    s32 saved;

    if (g_battle_thread_contexts[NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS].task_id
        != NATIVE_THREAD_TASK_ATTACK_CASTER_STATS) {
        battle_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    }
    battle_thread_yield();
    battle_thread_yield();
    saved = g_battle_active_turn_unit.battle_id;
    /* The target loads the preview unit id unsigned (lhu). */
    g_battle_active_turn_unit.battle_id = g_battle_preview_target_unit_id;
    battle_menu_load_selected_unit();
    g_battle_active_turn_unit.battle_id = saved;
    g_battle_menu_comparison_billboard_data.state.generic_monster = g_battle_menu_billboard_data.state.generic_monster;
    g_battle_menu_comparison_billboard_data.state.ability_ids[2] = g_battle_menu_billboard_data.state.ability_ids[2];
    battle_menu_load_selected_unit();
    battle_menu_display_hovered_unit_stats(&g_battle_menu_attack_target_stats_display, UNIT_STATS_PANEL_ATTACK_TARGET,
        (menu_number_entry_t*)g_battle_menu_attack_target_stats_setup, &g_battle_menu_status_billboard);
}
