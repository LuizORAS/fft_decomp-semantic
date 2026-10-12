#include "fft/battle_unit.h"
#include "psx/types.h"

/* The battle stats of an occupied unit slot, or 0 for an empty slot or an id past
 * the last slot. */
battle_stats_t* battle_unit_get_existing_pointer(u32 unit_id) {
    battle_stats_t* unit = 0;
    if (unit_id < BATTLE_UNIT_SLOT_COUNT) {
        unit = &g_battle_unit_stats[unit_id];
        if (unit->entd_slot == BATTLE_ENTD_SLOT_NONE) {
            unit = 0;
        }
    }
    return unit;
}
