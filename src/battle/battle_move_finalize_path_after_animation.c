#include "fft/battle_move.h"
#include "psx/types.h"

/* Step phase TELEPORT_ANIMATION (0x3c): once the unit's animation ends, finish its walk: clear
 * the step phase and the path count, keeping the count in last_path_count. */
void battle_move_finalize_path_after_animation(battle_unit_t* unit) {
    if (unit->animation.wait == 0) {
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_NONE;
        unit->last_path_count = unit->path.count;
        unit->path.count = 0;
    }
}
