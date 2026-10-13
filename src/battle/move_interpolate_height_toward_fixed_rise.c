#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "psx/libgte.h"

/* Distortion animation 0x02: raise the unit by 0x24000 (36 screen units) in equal
 * steps over distortion_timer frames.
 *
 * Phase 0 replaces the frame count in distortion_timer with the per-frame step
 * and stores the destination height in distortion_target; phase 1 steps until the
 * unit reaches it, then snaps and ends the animation. */
void move_interpolate_height_toward_fixed_rise(battle_unit_t* misc) {
    s32 step;
    s32 target;
    s32 height;

    step = misc->distortion_timer;
    target = misc->distortion_target;
    switch (misc->distortion_phase) {
    case 0:
        step = -0x24000 / step;
        target = misc->real.vy + -0x24000;
        misc->distortion_phase = 1;
    case 1:
        if (target < misc->real.vy) {
            misc->real.vy = step + misc->real.vy;
        } else {
            misc->real.vy = target;
            misc->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        height = misc->real.vy;
        misc->screen.vy = height / ONE;
        break;
    }
    misc->distortion_timer = step;
    misc->distortion_target = target;
}
