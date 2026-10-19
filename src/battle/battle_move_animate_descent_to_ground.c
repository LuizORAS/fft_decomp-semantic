#include "fft/battle_gfx.h"
#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Distortion 3 (BATTLE_DISTORTION_ANIMATION_SETTLE): lower the unit to the ground under it in
 * equal steps over the queued frames (phase 0 turns distortion_timer into the per-frame step and
 * stores the ground height in distortion_target), then end; a unit that is not above the ground
 * snaps to it at once. */
void battle_move_animate_descent_to_ground(battle_unit_t* unit) {
    s32 step;
    s32 z;
    s32 real_z;

    step = unit->distortion_timer;
    z = unit->distortion_target;
    switch (unit->distortion_phase) {
    case 0:
        z = battle_gfx_calculate_world_height_from_world_coords(unit) * ONE;
        step = (z - (unit->world_coords.vy * ONE)) / step;
        unit->distortion_phase = unit->distortion_phase + 1;
    case 1:
        if (unit->real.vy < z) {
            unit->real.vy = step + unit->real.vy;
        } else {
            unit->real.vy = z;
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        real_z = unit->real.vy;
        unit->world_coords.vy = real_z / ONE;
        break;
    }
    unit->distortion_timer = step;
    unit->distortion_target = z;
}
