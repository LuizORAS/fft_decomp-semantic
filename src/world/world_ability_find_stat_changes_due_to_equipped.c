#include "fft/world.h"

/* Stat preview for swapping one movement ability for another: clears
 * `stat_diff` and stores the difference in the Move and Jump bonuses the two
 * abilities give. */
void world_ability_find_stat_changes_due_to_equipped(
    world_item_stat_detail_t* stat_diff, s32 old_ability, s32 new_ability) {
    battle_unit_status_record_t summary;
    world_item_stat_detail_t old_stats;
    world_item_stat_detail_t new_stats;

    world_formation_clear_stat_preview(&summary, stat_diff);
    world_ability_get_move_and_jump_increase_values(old_ability, &old_stats);
    world_ability_get_move_and_jump_increase_values(new_ability, &new_stats);
    stat_diff->move_bonus = new_stats.move_bonus - old_stats.move_bonus;
    stat_diff->jump_bonus = new_stats.jump_bonus - old_stats.jump_bonus;
}
