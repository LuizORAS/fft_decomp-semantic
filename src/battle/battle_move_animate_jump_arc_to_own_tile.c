#include "fft/battle_gfx.h"
#include "fft/battle_move.h"
#include "fft/battle_unit.h"
#include "fft/main.h"
#include "fft/map.h"
#include "psx/types.h"

/* Distortion 7 (BATTLE_DISTORTION_ANIMATION_JUMP_TO_OWN_TILE): jump along an arc back to the center of the
 * unit's own map tile (battle_move_calculate_jump_arc_velocity), then keep falling under
 * g_battle_move_jump_gravity until it reaches the ground, and snap to the tile. */
void battle_move_animate_jump_arc_to_own_tile(battle_unit_t* unit) {
    /* Unreferenced 8-byte frame slot below the target point, like the one in
     * battle_move_animate_jump_start; it puts `target` at sp+0x18. */
    SVECTOR unused_10;
    SVECTOR target;
    VECTOR velocity;
    s32 timer;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        target.vx = unit->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        target.vz = unit->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
        target.vy = battle_gfx_calculate_world_height_from_unit_tile(unit);
        timer = battle_move_calculate_jump_arc_velocity(&unit->world_coords, &target, &velocity);
        unit->distortion_phase++;
    case 1:
        if (timer > 0) {
            unit->real.vx += velocity.vx;
            unit->real.vy += velocity.vy;
            unit->real.vz += velocity.vz;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            velocity.vz = 0;
            velocity.vx = 0;
            unit->distortion_phase++;
        }
        timer--;
        break;
    case 2:
        main_util_set_svector(&target, unit->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2), 0,
            unit->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2));
        target.vy = battle_gfx_calculate_world_height_from_world_coords(unit);
        if (unit->world_coords.vy < target.vy) {
            unit->real.vy += velocity.vy;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            battle_unit_set_real_coords_from_map_coords(unit);
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_world_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
