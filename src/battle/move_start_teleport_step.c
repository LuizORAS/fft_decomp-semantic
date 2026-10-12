#include "fft/battle_effect.h"

/* Puts the unit in the TELEPORT_START step phase. The init record it builds is not used. */
void move_start_teleport_step(battle_unit_misc_data_t* unit) {
    effect_secondary_init_t data;

    effect_build_secondary_init(unit, &data);
    unit->step_phase = MOVE_STEP_PHASE_TELEPORT_START;
}
