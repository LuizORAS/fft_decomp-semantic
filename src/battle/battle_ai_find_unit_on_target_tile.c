#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Return the first present unit's slot on the current action's target tile (target_x, _y,
 * _elevation), or BATTLE_UNIT_ID_NONE: battle_ai_find_unit_at_coordinates for those fields. */
s32 battle_ai_find_unit_on_target_tile(void) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    s32 i;
    battle_stats_t* unit;
    u16 x;
    u16 y;
    u16 elevation;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = &g_battle_unit_stats[i];
        if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
            continue;
        }
        x = ai->current_action.command.target_position.vx;
        if (unit->x != x) {
            continue;
        }
        y = ai->current_action.command.target_position.vz;
        if (unit->position.y != y) {
            continue;
        }
        elevation = ai->current_action.command.target_position.vy;
        if (unit->position.higher_elevation == elevation) {
            break;
        }
    }
    return i;
}
