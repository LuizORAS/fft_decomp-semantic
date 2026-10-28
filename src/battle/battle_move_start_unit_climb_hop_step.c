#include "fft/battle_move.h"
#include "psx/types.h"

/* Start a unit's hop onto a nearby tile (from battle_move_update_walking_step_at_tile_edge):
 * derive the launch rise and flight frames from the landing height, then set the step phase,
 * facing, destination tile and horizontal velocity for the step's direction.
 *
 * Case bodies store the destination before clearing the other velocity axis;
 * that order gives the target's map_x/step load order. */
void battle_move_start_unit_climb_hop_step(battle_unit_t* unit, const map_tile_t* from, const map_tile_t* to) {
    s32 unused[4];
    battle_unit_height_data_t height_data;
    s32 rise;
    s32 frames;
    battle_move_step_t* step;
    s32 rise_height;
    s32 height;
    u32 animation;
    u8 next;

    step = &unit->current_path_step;
    if (unit->standing_on_battle_id_plus_one != 0
        && (battle_unit_calculate_height_data(&height_data, unit->standing_on_battle_id_plus_one - 1),
            (height_data.unit_flags & BATTLE_UNIT_HEIGHT_UNAVAILABLE) == 0)) {
        height = height_data.total_height;
    } else {
        height = to->height * 2 + to->depth_half_height.half_height * unit->destination_edge_height;
    }
    rise_height = (height - -unit->world_coords.vy / 6) * 6 + 1;
    rise = SquareRoot12(rise_height * g_battle_move_jump_gravity * 2);
    unit->velocity.vy = -rise;
    frames = rise / g_battle_move_jump_gravity;
    switch (step->direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_EAST;
        unit->facing = BATTLE_FACING_EAST;
        next = unit->map_x + 1;
        unit->movement.destination_x = next + step->length;
        unit->velocity.vz = 0;
        unit->velocity.vx = (BATTLE_MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_WEST;
        unit->facing = BATTLE_FACING_WEST;
        next = unit->map_x - 1;
        unit->movement.destination_x = next - step->length;
        unit->velocity.vz = 0;
        unit->velocity.vx = -(BATTLE_MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_SOUTH;
        unit->facing = BATTLE_FACING_SOUTH;
        next = unit->map_y - 1;
        unit->movement.destination_y = next - step->length;
        unit->velocity.vx = 0;
        unit->velocity.vz = -(BATTLE_MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        /* Without this reorg moves this case's `li a0,0xe000` into the
         * `beq a1,3` delay slot, where the target keeps a nop; an asm insn
         * stops fill_slots_from_thread's scan (stop_search_p). */
        __asm__ volatile("");
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH;
        unit->facing = BATTLE_FACING_NORTH;
        next = unit->map_y + 1;
        unit->movement.destination_y = next + step->length;
        unit->velocity.vx = 0;
        unit->velocity.vz = (BATTLE_MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    }
    animation = BATTLE_UNIT_ANIMATION_HOP;
    if (unit->movement.alternate_short_hop) {
        animation = BATTLE_UNIT_ANIMATION_HOP_ALTERNATE;
    }
    battle_unit_store_animation_facing(animation, unit->facing, unit);
    unit->movement.alternate_short_hop ^= 1;
    unit->movement.destination_z = step->higher;
}
