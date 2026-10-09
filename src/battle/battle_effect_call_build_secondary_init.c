#include "fft/battle.h"

void battle_effect_call_build_secondary_init(battle_unit_misc_data_t* unit) {
    battle_effect_secondary_init_t data;

    battle_effect_build_secondary_init(unit, &data);
    unit->step_phase = BATTLE_MOVE_STEP_PHASE_TELEPORT_START;
}
