#include "fft/battle.h"
#include "psx/types.h"

/* Step handler of the TELEPORT_ANIMATION phase: once the animation countdown
 * reaches 0, end the path and keep its count in last_path_count. */
void battle_move_finalize_path_after_animation(battle_unit_misc_data_t* unit) {
    u8 path_count;

    if (unit->animation.wait == 0) {
        path_count = unit->path.count;
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_NONE;
        unit->path.count = 0;
        unit->last_path_count = path_count;
    }
}
