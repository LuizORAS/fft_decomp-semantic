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
void move_interpolate_height_toward_resting_height(battle_unit_t* unit) {
    s32 step;
    s32 z;
    s32 real_z;

    step = unit->distortion_timer;
    z = unit->distortion_target;
    switch (unit->distortion_phase) {
    case 0:
        z = battle_gfx_calculate_world_height_from_world_coords(unit) << 12;
        step = (z - (unit->world_coords.vy << 12)) / step;
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
