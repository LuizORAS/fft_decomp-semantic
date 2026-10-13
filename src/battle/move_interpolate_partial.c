#include "fft/battle_move.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Scales the misc unit's partial X/Z/Y step by a walk speed in ONE-based
 * fixed point. */
void move_interpolate_partial(battle_unit_t* unit, s32 scale) {
    unit->velocity.vx = (scale * unit->velocity.vx) / ONE;
    unit->velocity.vy = (scale * unit->velocity.vy) / ONE;
    unit->velocity.vz = (scale * unit->velocity.vz) / ONE;
}
