#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Index of the first present unit standing on the current action's target
 * panel, or BATTLE_UNIT_ID_NONE if none. */
s32 ai_find_unit_on_target_panel(void) {
    ai_data_t* ai = &g_ai_data_base;
    s32 i;
    unit_stats_t* unit;
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
