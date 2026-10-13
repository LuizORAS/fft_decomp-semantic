#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_preview_attack_target_stats. */
void world_menu_preview_attack_target_stats(void) {
    s32 saved;

    if (g_world_thread_contexts[NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS].task_id
        != NATIVE_THREAD_TASK_ATTACK_CASTER_STATS) {
        world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    }
    world_thread_yield();
    world_thread_yield();
    saved = g_world_active_turn_unit.battle_id;
    g_world_active_turn_unit.battle_id = g_world_preview_target_unit_id;
    world_menu_load_selected_unit();
    g_world_active_turn_unit.battle_id = saved;
    g_world_menu_comparison_billboard_data.state.generic_monster = g_world_menu_billboard_data.state.generic_monster;
    g_world_menu_comparison_billboard_data.state.ability_ids[2] = g_world_menu_billboard_data.state.ability_ids[2];
    world_menu_load_selected_unit();
    world_menu_display_hovered_unit_stats(&g_world_menu_attack_target_stats_display, UNIT_STATS_PANEL_ATTACK_TARGET,
        g_world_menu_attack_target_stats_setup, &g_world_menu_status_billboard);
}
