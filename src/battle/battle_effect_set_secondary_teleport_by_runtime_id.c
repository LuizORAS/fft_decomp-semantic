#include "fft/battle_effect.h"
#include "fft/battle_unit.h"

/* Show the teleport effect on the unit whose runtime id is the low 16 bits of runtime_id and return 1, or 0
 * when there is none. The event instructions that teleport a unit out and in call it. */
s32 battle_effect_set_secondary_teleport_by_runtime_id(u32 runtime_id) {
    battle_unit_t* unit;

    unit = battle_unit_get_by_runtime_id(runtime_id);
    if (unit != 0) {
        battle_effect_set_secondary_teleport(unit);
        return 1;
    }
    return 0;
}
