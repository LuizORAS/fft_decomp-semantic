#include "fft/menu.h"
#include "fft/world.h"

/* Fill stat_diff with the Move and Jump change of replacing movement ability old_ability with
 * new_ability, every other stat cleared; the ability-removal menu (world_formation_run_remove_ability_menu)
 * previews clearing the Move+ or Jump+ slot with it. */
void world_ability_find_stat_changes_due_to_equipped(
    world_item_stat_detail_t* stat_diff, s32 old_ability, s32 new_ability) {
    battle_menu_unit_status_record_t summary;
    world_item_stat_detail_t old_stats;
    world_item_stat_detail_t new_stats;

    menu_clear_stat_preview(&summary, stat_diff);
    world_ability_get_move_and_jump_increase_values(old_ability, &old_stats);
    world_ability_get_move_and_jump_increase_values(new_ability, &new_stats);
    stat_diff->state.move = new_stats.state.move - old_stats.state.move;
    stat_diff->state.jump = new_stats.state.jump - old_stats.state.jump;
}
