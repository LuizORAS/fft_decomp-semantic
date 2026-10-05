#include "fft/battle.h"

/* Distortion 0xf (BATTLE_DISTORTION_SLIDE_BACK): slide the unit away from its first target, or from
 * the centre of the action target tile, over distortion_timer frames, by seven eighths of the
 * distance between them wrapped into 0xe000 (half a tile), at a constant height. A unit with no
 * battle record ends at once.
 *
 * Clearing velocity.vy in each branch keeps its division, which GCC folds away when the zero is
 * stored once after the branches. `unused_10` reproduces an unreferenced 8-byte frame slot below
 * the velocity copy. */
void battle_unit_animate_slide_back_from_target(battle_unit_misc_data_t* unit) {
    s32 unused_10[2];
    VECTOR velocity;
    battle_unit_misc_data_t* target;
    s32 timer;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        if (unit->battle_data != 0) {
            if (unit->target_count != 0) {
                target = battle_unit_get_misc_data_by_battle_id(unit->target_list[0]);
                velocity.vx = unit->real.vx - target->real.vx;
                velocity.vx = (velocity.vx - velocity.vx / 8) % 0xe000;
                velocity.vz = unit->real.vz - target->real.vz;
                velocity.vz = (velocity.vz - velocity.vz / 8) % 0xe000;
                velocity.vy = 0;
            } else {
                velocity.vx = (unit->screen.vx - (unit->battle_data->action_target_x * 28 + 0xe)) << 12;
                velocity.vx = (velocity.vx - velocity.vx / 8) % 0xe000;
                velocity.vz = (unit->screen.vz - (unit->battle_data->action_target_y * 28 + 0xe)) << 12;
                velocity.vz = (velocity.vz - velocity.vz / 8) % 0xe000;
                velocity.vy = 0;
            }
            velocity.vx /= timer;
            velocity.vy /= timer;
            velocity.vz /= timer;
            unit->distortion_phase++;
        } else {
            unit->distortion_animation_id = 0;
            break;
        }
    case 1:
        if (timer > 0) {
            unit->real.vx += velocity.vx;
            unit->real.vz += velocity.vz;
        } else {
            unit->distortion_animation_id = 0;
        }
        timer--;
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_screen_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
