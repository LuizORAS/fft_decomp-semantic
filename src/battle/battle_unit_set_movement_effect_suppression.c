#include "fft/battle_unit.h"

/* Set BATTLE_MOTION_FLAG_SUPPRESS_SFX_AND_LANDING_EFFECTS on the unit with this runtime id: its steps make
 * no sound and its landings no effect (script_set_units_movement_effect_suppression). */
void battle_unit_set_movement_effect_suppression(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit != 0) {
        unit->movement.suppress_sfx_and_landing_effects = 1;
    }
}
