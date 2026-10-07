#include "fft/world.h"

/* Fill stat_diff with the Move and Jump change of replacing movement ability old_ability with
 * new_ability, every other stat cleared; the ability-removal menu (world_menu_run_remove_ability)
 * previews clearing the Move+ or Jump+ slot with it. */
void world_ability_find_stat_changes_due_to_equipped(
    world_item_stat_detail_t* stat_diff, s16 old_ability, s16 new_ability) {
    world_item_stat_summary_t summary;
    world_item_stat_detail_t old_stats;
    world_item_stat_detail_t new_stats;

    world_formation_clear_stat_preview(&summary, stat_diff);
    world_ability_get_move_and_jump_increase_values(old_ability, &old_stats);
    world_ability_get_move_and_jump_increase_values(new_ability, &new_stats);
    stat_diff->move_bonus = (u16)new_stats.move_bonus - (u16)old_stats.move_bonus;
    stat_diff->jump_bonus = (u16)new_stats.jump_bonus - (u16)old_stats.jump_bonus;
}
