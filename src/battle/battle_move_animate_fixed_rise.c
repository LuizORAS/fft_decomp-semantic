#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "psx/libgte.h"

/* Distortion 2 (BATTLE_DISTORTION_ANIMATION_RISE): raise the unit 36 world units (three height levels) at
 * a constant speed over the queued frames, then end; phase 0 turns distortion_timer into the per-frame
 * step and distortion_target into the end height. Monster SEQ animations pair it with
 * battle_move_animate_descent_to_ground. */
void battle_move_animate_fixed_rise(battle_unit_t* unit) {
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
