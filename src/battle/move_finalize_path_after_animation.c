#include "fft/battle_move.h"
#include "psx/types.h"

/* Step handler of the TELEPORT_ANIMATION phase: once the animation countdown
 * reaches 0, end the path and keep its count in last_path_count. */
void move_finalize_path_after_animation(battle_unit_t* unit) {
    if (unit->animation.wait == 0) {
        unit->step_phase = MOVE_STEP_PHASE_NONE;
        unit->last_path_count = unit->path.count;
        unit->path.count = 0;
    }
}
