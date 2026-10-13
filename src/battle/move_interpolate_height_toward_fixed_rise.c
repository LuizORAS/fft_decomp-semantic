#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "psx/libgte.h"

/* Distortion animation 0x02: raise the unit by 0x24000 (36 world units) in equal
 * steps over distortion_timer frames.
 *
 * Phase 0 replaces the frame count in distortion_timer with the per-frame step
 * and stores the destination height in distortion_target; phase 1 steps until the
 * unit reaches it, then snaps and ends the animation. */
void move_interpolate_height_toward_fixed_rise(battle_unit_t* unit) {
    s32 step;
    s32 target;
    s32 height;

    step = unit->distortion_timer;
    target = unit->distortion_target;
    switch (unit->distortion_phase) {
    case 0:
        step = -0x24000 / step;
        target = unit->real.vy + -0x24000;
        unit->distortion_phase = 1;
    case 1:
        if (target < unit->real.vy) {
            unit->real.vy = step + unit->real.vy;
        } else {
            unit->real.vy = target;
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        height = unit->real.vy;
        unit->world_coords.vy = height / ONE;
        break;
    }
    unit->distortion_timer = step;
    unit->distortion_target = target;
}
