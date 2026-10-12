#include "fft/battle_gfx.h"
#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Distortion animation 0x03: lower the unit to the ground height under it in
 * equal steps over distortion_timer frames.
 *
 * Phase 0 stores the ground height (Q12) in distortion_target and replaces the
 * frame count in distortion_timer with the per-frame step; phase 1 steps while
 * the unit is above the ground, then snaps and ends the animation. A unit that
 * starts at or below the ground snaps on the first frame. */
void move_interpolate_height_toward_resting_height(battle_unit_misc_data_t* misc) {
    s32 step;
    s32 z;
    s32 real_z;

    step = misc->distortion_timer;
    z = misc->distortion_target;
    switch (misc->distortion_phase) {
    case 0:
        z = battle_gfx_calculate_screen_z_from_misc_screen_data(misc) << 12;
        step = (z - (misc->screen.vy << 12)) / step;
        misc->distortion_phase = misc->distortion_phase + 1;
    case 1:
        if (misc->real.vy < z) {
            misc->real.vy = step + misc->real.vy;
        } else {
            misc->real.vy = z;
            misc->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        real_z = misc->real.vy;
        misc->screen.vy = real_z / ONE;
        break;
    }
    misc->distortion_timer = step;
    misc->distortion_target = z;
}
