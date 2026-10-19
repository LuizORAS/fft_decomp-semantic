#include "fft/battle_map.h"
#include "fft/battle_script.h"
#include "fft/script.h"

/* Pick the rare or common move-find item on the unit's tile and mark the tile found.
 *
 * The rare item is chosen with a (100 - Brave)% chance; the tile is the
 * unit's x and position (y in the low byte, the layer in bit 15). */
battle_move_found_item_t* battle_map_determine_rare_common_item(battle_stats_t* unit) {
    battle_move_find_result_data_t* find_result;
    u8 item_id; /* 0x8018f8a4; ID of the found item */

    find_result = battle_map_get_move_find_result(unit->x, unit->position.y, unit->position.higher_elevation);
    if (main_util_roll_pass_fail(0x64, 0x64 - unit->brave) == 0) {
        item_id = find_result->rare_item_id;
    } else {
        item_id = find_result->common_item_id;
    }
    g_battle_map_rare_item_result.item_id = item_id;
    battle_map_calculate_move_find_item_flag(
        script_get_variable(EVENT_SCRIPT_VAR_CURRENT_MAP), find_result->entry_index, 1);
    g_battle_map_rare_item_result._unknown_00 = 0;
    return &g_battle_map_rare_item_result;
}
