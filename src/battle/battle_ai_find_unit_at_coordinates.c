#include "fft/battle_ai.h"
#include "fft/battle_unit.h"

/* Return the first present unit's slot at these coordinates, or BATTLE_UNIT_ID_NONE if absent. */
s32 battle_ai_find_unit_at_coordinates(const battle_ai_coords_t* coords) {
    s32 i;
    battle_stats_t* unit;

    for (i = 0; i < BATTLE_UNIT_SLOT_COUNT; i++) {
        unit = &g_battle_unit_stats[i];
        if (unit->entd_slot != BATTLE_ENTD_SLOT_NONE && unit->x == coords->fields.x
            && unit->position.y == coords->fields.y && (unit->position.higher_elevation) == coords->fields.elevation) {
            break;
        }
    }
    return i;
}
