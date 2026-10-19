#include "fft/battle_effect.h"
#include "fft/battle_move.h"

/* Puts the unit in the TELEPORT_START step phase. The init record it builds is not used. */
void battle_move_start_teleport_step(battle_unit_t* unit) {
    battle_effect_secondary_init_t data;

    battle_effect_build_secondary_init(unit, &data);
    unit->step_phase = BATTLE_MOVE_STEP_PHASE_TELEPORT_START;
}
