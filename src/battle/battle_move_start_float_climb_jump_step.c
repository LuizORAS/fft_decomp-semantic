#include "fft/battle.h"
#include "psx/types.h"

/* Start a unit's climb jump from its current tile onto the higher tile `to`.
 *
 * The rise velocity is sqrt(2 * gravity * height) for the height difference in
 * half-tile steps (6 screen units each) plus two half-steps per tile of jump
 * length; a second apex term extends the frame count. The step byte's top two
 * bits pick the direction, which sets the centre-tile offset, facing, the
 * destination coordinate, and a half-tile horizontal displacement spread over
 * the frame count. `from` is unused. `unused` reproduces an unreferenced 24-byte
 * frame slot. The separate `height -=`/`height +=` statements keep the target's
 * in-place accumulation register; `rise_height` and `apex` keep the multiplication
 * order (term * 6 or 12 before gravity) that one folded expression reassociates.
 */
void battle_move_start_float_climb_jump_step(
    battle_unit_misc_data_t* unit, const map_tile_t* from, const map_tile_t* to) {
    battle_move_step_t current_step;
    s32 rise;
    s32 frames;
    battle_move_step_t* step;
    s32 unused[6];
    s32 rise_height;
    s32 height;
    s32 apex;
    u8 next;

    step = &unit->current_path_step;
    height = to->height * 2 + to->depth_half_height.bits.half_height * unit->destination_edge_height + 2;
    height -= -unit->screen.vy / 6;
    current_step = unit->current_path_step;
    height += (current_step.bits.length + 1) * 2;
    rise_height = height * 6;
    rise = SquareRoot12(rise_height * g_battle_move_jump_gravity * 2);
    frames = rise / g_battle_move_jump_gravity;
    unit->velocity.vy = -rise;
    current_step = unit->current_path_step;
    apex = (current_step.bits.length + 1) * 12;
    frames += SquareRoot12(apex * g_battle_move_jump_gravity * 2) / g_battle_move_jump_gravity;
    /* The byte copy prevents a wider direction load. */
    current_step = unit->current_path_step;
    switch (current_step.bits.direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_EAST;
        unit->facing = BATTLE_FACING_EAST;
        unit->velocity.vz = 0;
        next = unit->map_x + 1;
        unit->movement.destination_x = next + unit->current_path_step.bits.length;
        unit->velocity.vx = (MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_WEST;
        unit->facing = BATTLE_FACING_WEST;
        unit->velocity.vz = 0;
        next = unit->map_x - 1;
        unit->movement.destination_x = next - unit->current_path_step.bits.length;
        unit->velocity.vx = -(MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_SOUTH;
        unit->facing = BATTLE_FACING_SOUTH;
        unit->velocity.vx = 0;
        next = unit->map_y - 1;
        unit->movement.destination_y = next - unit->current_path_step.bits.length;
        unit->velocity.vz = -(MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        unit->step_phase = BATTLE_MOVE_STEP_PHASE_FLOAT_JUMP_NORTH;
        unit->facing = BATTLE_FACING_NORTH;
        unit->velocity.vx = 0;
        next = unit->map_y + 1;
        unit->movement.destination_y = next + unit->current_path_step.bits.length;
        unit->velocity.vz = (MAP_TILE_SIZE * ONE / 2) / frames;
        break;
    }
    unit->movement.destination_z = step->bits.higher;
}
