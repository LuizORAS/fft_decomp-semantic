#include "fft/battle_unit.h"

void battle_unit_clear_movement_effect_suppression(u32 misc_id) {
    battle_unit_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    if (unit != 0) {
        unit->movement.suppress_sfx_and_landing_effects = 0;
    }
}
