#include "fft/world.h"
#include "psx/types.h"

/*
 * Attack-preview target panel: waits for the caster panel thread unless it
 * is already the attack-caster task, then shows the target panel.
 *
 * world_menu_load_selected_unit runs twice: once with the target as the
 * active unit, to copy its monster flag and third ability row into the
 * comparison fields, and again with the active unit restored.
 */
void world_menu_preview_attack_target_stats(void) {
    s32 saved;

    if (g_world_thread_contexts[NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS].task_id
        != NATIVE_THREAD_TASK_ATTACK_CASTER_STATS) {
        world_thread_wait_until_inactive(NATIVE_THREAD_SLOT_RIGHT_UNIT_STATS);
    }
    world_thread_yield();
    world_thread_yield();
    saved = g_world_active_unit_data.battle_id;
    g_world_active_unit_data.battle_id = g_world_unit_comparison_battle_id;
    world_menu_load_selected_unit();
    g_world_active_unit_data.battle_id = saved;
    g_world_unit_comparison_editor_fields.state.generic_monster = g_world_unit_editor_fields.state.generic_monster;
    g_world_unit_comparison_editor_fields.state.ability_ids[2] = g_world_unit_editor_fields.state.ability_ids[2];
    world_menu_load_selected_unit();
    world_menu_display_hovered_unit_stats(&g_world_menu_attack_target_stats_display, UNIT_STATS_PANEL_ATTACK_TARGET,
        g_world_menu_attack_target_stats_setup, &g_world_unit_comparison_status_billboard);
}
