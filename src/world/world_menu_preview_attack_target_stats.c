#include "fft/world.h"
#include "psx/types.h"

void world_menu_preview_attack_target_stats(void) {
    s32 saved;

    if (g_world_thread_contexts[12].task_id != 0x13) {
        world_thread_wait_until_inactive(0xC);
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
    world_menu_display_hovered_unit_stats(&g_world_menu_attack_target_stats_display, 2,
        g_world_menu_attack_target_stats_setup, &g_world_unit_comparison_status_billboard);
}
