#include "fft/battle_unit.h"
#include "fft/map.h"

/* Distortion 0xf (BATTLE_DISTORTION_ANIMATION_BACK_AWAY): slide the unit away from its first target, or from
 * the centre of the action target tile, over distortion_timer frames, by seven eighths of the
 * distance between them wrapped into 0xe000 (half a tile), at a constant height. A unit with no
 * battle record ends at once.
 *
 * Clearing velocity.vy in each branch keeps its division, which GCC folds away when the zero is
 * stored once after the branches. `unused_10` reproduces an unreferenced 8-byte frame slot below
 * the velocity copy. */
void battle_unit_animate_slide_back_from_target(battle_unit_t* unit) {
    s32 unused_10[2];
    VECTOR velocity;
    battle_unit_t* target;
    s32 timer;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        if (unit->stats != 0) {
            if (unit->strike.target_count != 0) {
                target = battle_unit_get_by_battle_id(unit->strike.target_list[0]);
                velocity.vx = unit->real.vx - target->real.vx;
                velocity.vx = (velocity.vx - velocity.vx / 8) % 0xe000;
                velocity.vz = unit->real.vz - target->real.vz;
                velocity.vz = (velocity.vz - velocity.vz / 8) % 0xe000;
                velocity.vy = 0;
            } else {
                velocity.vx = (unit->world_coords.vx
                                  - (unit->stats->action_command.target_position.vx * BATTLE_MAP_TILE_SIZE
                                      + (BATTLE_MAP_TILE_SIZE / 2)))
                    * ONE;
                velocity.vx = (velocity.vx - velocity.vx / 8) % 0xe000;
                velocity.vz = (unit->world_coords.vz
                                  - (unit->stats->action_command.target_position.vz * BATTLE_MAP_TILE_SIZE
                                      + (BATTLE_MAP_TILE_SIZE / 2)))
                    * ONE;
                velocity.vz = (velocity.vz - velocity.vz / 8) % 0xe000;
                velocity.vy = 0;
            }
            velocity.vx /= timer;
            velocity.vy /= timer;
            velocity.vz /= timer;
            unit->distortion_phase++;
        } else {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
            break;
        }
    case 1:
        if (timer > 0) {
            unit->real.vx += velocity.vx;
            unit->real.vz += velocity.vz;
        } else {
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        timer--;
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_world_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
