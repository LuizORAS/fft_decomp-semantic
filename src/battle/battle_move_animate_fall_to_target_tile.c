#include "fft/battle.h"
#include "psx/types.h"

/* The target sets $a1 to the coordinate buffer before this call too, so the
 * original declared both world-height helpers with the same two arguments. */
#define world_height_from_unit_world_coords                                                                            \
    ((s32 (*)(battle_unit_t*, const SVECTOR*))battle_gfx_calculate_world_height_from_world_coords)

/* Distortion 0xd (BATTLE_DISTORTION_ANIMATION_FALL_TO_TARGET): drop the unit under gravity, straight down
 * from where it is, onto the ground of its first target's tile or else of the action target tile,
 * and end there. During an event it falls onto its own tile instead, held at the tile's centre (see
 * QUIRKS.md for the coordinate it writes). */
void battle_move_animate_fall_to_target_tile(battle_unit_t* unit) {
    SVECTOR coords;
    /* Unreferenced 8-byte frame slot above `coords`; it puts `velocity` at
     * sp+0x20, matching the sibling distortion handlers. */
    SVECTOR unused_18;
    VECTOR velocity;
    battle_unit_t* other;
    battle_stats_t* stats;
    s32 timer;
    s32 x;
    s32 y;

    timer = unit->distortion_timer;
    velocity = unit->velocity;
    switch (unit->distortion_phase) {
    case 0:
        velocity.vy = g_battle_move_jump_gravity;
        battle_gfx_init_position_vector_copies(unit);
        unit->shadow_graphic_trigger = 1;
        unit->distortion_phase++;
    case 1:
        if (g_battle_game_state == BATTLE_GAME_STATE_EVENT) {
            unit->strike.target_count = 0;
            x = unit->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
            unit->world_coords.vx = x;
            unit->real.vx = x * ONE;
            y = unit->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2);
            unit->world_coords.vz = y;
            /* Retail stores the tile-centre depth into real.vy (the height),
             * not real.vz. The first frame adds the fall speed to it and the
             * next one snaps the height to the ground. */
            unit->real.vy = y * ONE;
            main_util_set_svector(&coords, unit->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2), unit->map_z,
                unit->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2));
            coords.vy = world_height_from_unit_world_coords(unit, &coords);
        } else {
            if (unit->strike.target_count != 0) {
                other = battle_unit_get_by_battle_id(unit->strike.target_list[0]);
                main_util_set_svector(&coords, other->map_x * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2),
                    other->map_z, other->map_y * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2));
            } else {
                stats = unit->stats;
                main_util_set_svector(&coords,
                    stats->action_command.target_position.vx * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2),
                    stats->action_command.target_position.vy,
                    stats->action_command.target_position.vz * BATTLE_MAP_TILE_SIZE + (BATTLE_MAP_TILE_SIZE / 2));
            }
            coords.vy = battle_gfx_calculate_world_height_with_caller_coords(unit, &coords);
        }
        if (unit->world_coords.vy < coords.vy) {
            unit->real.vy += velocity.vy;
            velocity.vy += g_battle_move_jump_gravity;
        } else {
            unit->real.vy = coords.vy * ONE;
            unit->position_copies_active = 0;
            unit->distortion_animation_id = BATTLE_DISTORTION_ANIMATION_NONE;
        }
        break;
    }
    unit->velocity = velocity;
    battle_unit_set_world_coords_from_real_coords(unit);
    unit->distortion_timer = timer;
}
