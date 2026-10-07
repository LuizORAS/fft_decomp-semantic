#include "fft/battle.h"
#include "psx/types.h"

/* Distortion 3 (BATTLE_DISTORTION_DESCEND_TO_GROUND): lower the unit to the ground under it at a
 * constant speed over the queued frames, then end; a unit that is not above the ground snaps to it
 * at once. */
void battle_unit_animate_descent_to_ground(battle_unit_misc_data_t* misc) {
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
            misc->distortion_animation_id = 0;
        }
        real_z = misc->real.vy;
        misc->screen.vy = real_z / ONE;
        break;
    }
    misc->distortion_timer = step;
    misc->distortion_target = z;
}
