#include "fft/battle.h"

void battle_gfx_init_status_bubble(battle_unit_misc_data_t* unit) {
    u8 tile_offset;

    tile_offset = unit->step_phase;
    if (tile_offset != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_SOUTH && tile_offset != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_EAST
        && tile_offset != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_WEST
        && tile_offset != BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_NORTH) {
        battle_unit_increment_or_decrement_height_mod(unit);
    }
    if (unit->status_bubble_active != 0) {
        battle_gfx_determine_status_bubble_parameters(unit);
    }
}
