#include "fft/battle_effect.h"
#include "fft/battle_unit.h"

s32 effect_set_secondary_teleport_by_runtime_id(u32 runtime_id) {
    battle_unit_t* unit;

    unit = battle_unit_get_by_runtime_id(runtime_id);
    if (unit != 0) {
        effect_set_secondary_teleport(unit);
        return 1;
    }
    return 0;
}
