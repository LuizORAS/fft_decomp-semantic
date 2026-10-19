#include "fft/battle_unit.h"

/* Clear BATTLE_MOTION_FLAG_SUPPRESS_SFX_AND_LANDING_EFFECTS on the unit with this runtime id
 * (script_set_units_movement_effect_suppression). */
void battle_unit_clear_movement_effect_suppression(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit != 0) {
        unit->movement.suppress_sfx_and_landing_effects = 0;
    }
}
