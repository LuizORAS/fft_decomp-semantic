#include "fft/menu.h"
#include "fft/world.h"

/* Stat preview for swapping one movement ability for another: clears
 * `stat_diff` and stores the difference in the Move and Jump bonuses the two
 * abilities give. */
void world_ability_find_stat_changes_due_to_equipped(
    menu_status_panel_slot_storage_t* stat_diff, s32 old_ability, s32 new_ability) {
    menu_unit_status_record_t summary;
    menu_status_panel_slot_storage_t old_stats;
    menu_status_panel_slot_storage_t new_stats;

    menu_clear_stat_preview(&summary, stat_diff);
    world_ability_get_move_and_jump_increase_values(old_ability, &old_stats);
    world_ability_get_move_and_jump_increase_values(new_ability, &new_stats);
    stat_diff->state.move = new_stats.state.move - old_stats.state.move;
    stat_diff->state.jump = new_stats.state.jump - old_stats.state.jump;
}
