#include "fft/battle.h"
#include "psx/types.h"

void battle_move_finalize_path_after_animation(battle_unit_misc_data_t* unit) {
    u8 path_count;

    if (unit->animation_wait == 0) {
        path_count = unit->movement_path_count;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_NONE;
        unit->movement_path_count = 0;
        unit->last_path_count = path_count;
    }
}
