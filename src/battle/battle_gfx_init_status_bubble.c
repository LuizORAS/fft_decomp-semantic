#include "fft/battle.h"

void battle_gfx_init_status_bubble(battle_unit_misc_data_t* unit) {
    u8 phase;

    phase = unit->step_phase;
    if (phase != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_SOUTH && phase != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_EAST
        && phase != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_WEST && phase != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_NORTH) {
        battle_unit_increment_or_decrement_height_mod(unit);
    }
    if (unit->status_bubble_active != 0) {
        battle_gfx_determine_status_bubble_parameters(unit);
    }
}
