#include "fft/battle_move.h"
#include "psx/types.h"

/* Start a unit's jump step toward the next path tile.
 *
 * The step byte's low two bits give the jump length in tiles and its top two
 * bits the direction. The vertical velocity scales with the length; a long
 * jump (length 2+) uses animation 0x1e with a per-direction facing, while a
 * short hop dismounts any rider and toggles movement bit 27 to alternate the
 * 0x30/0x31 hop animation. */
void move_start_unit_jump_step(battle_unit_t* unit, const map_tile_t* unused_from, const map_tile_t* unused_to) {
    u16 facings[4] = { BATTLE_FACING_EAST, BATTLE_FACING_WEST, BATTLE_FACING_SOUTH, BATTLE_FACING_NORTH };
    u8 next;
    u8 jump_phases[4] = { MOVE_STEP_PHASE_JUMP_START_EAST, MOVE_STEP_PHASE_JUMP_START_WEST,
        MOVE_STEP_PHASE_JUMP_START_SOUTH, MOVE_STEP_PHASE_JUMP_START_NORTH };
    u8 hop_phases[4] = { MOVE_STEP_PHASE_JUMP_ASCENT_EAST, MOVE_STEP_PHASE_JUMP_ASCENT_WEST,
        MOVE_STEP_PHASE_JUMP_ASCENT_SOUTH, MOVE_STEP_PHASE_JUMP_ASCENT_NORTH };
    move_step_t* step;
    s32 animation;
    move_step_t current_step;

    /* Copy the byte to retain narrow loads of the packed step fields. */
    current_step = unit->current_path_step;
    unit->velocity.vy = -g_move_jump_gravity
        * (((current_step.length * (MAP_TILE_SIZE * ONE)) + (MAP_TILE_SIZE * ONE / 2)) / (6 * ONE));
    step = &unit->current_path_step;
    current_step = unit->current_path_step;
    if (current_step.length >= 2) {
        unit->step_phase = jump_phases[current_step.direction];
        current_step = unit->current_path_step;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_JUMP, facings[current_step.direction], unit);
    } else {
        unit->step_phase = hop_phases[current_step.direction];
        battle_unit_dismount_rider(unit);
        animation = BATTLE_UNIT_ANIMATION_HOP;
        if (unit->movement.alternate_short_hop) {
            animation = BATTLE_UNIT_ANIMATION_HOP_ALTERNATE;
        }
        battle_unit_store_animation_facing(animation, unit->facing, unit);
        unit->movement.alternate_short_hop ^= 1;
    }
    switch (step->direction) {
    case MOVE_STEP_DIRECTION_EAST:
        next = unit->map_x + 1;
        unit->movement.destination_x = next + step->length;
        unit->velocity.vx = 3 * ONE;
        unit->velocity.vz = 0;
        break;
    case MOVE_STEP_DIRECTION_WEST:
        next = unit->map_x - 1;
        unit->movement.destination_x = next - step->length;
        unit->velocity.vx = -3 * ONE;
        unit->velocity.vz = 0;
        break;
    case MOVE_STEP_DIRECTION_SOUTH:
        next = unit->map_y - 1;
        unit->movement.destination_y = next - step->length;
        unit->velocity.vx = 0;
        unit->velocity.vz = -3 * ONE;
        break;
    case MOVE_STEP_DIRECTION_NORTH:
        next = unit->map_y + 1;
        unit->movement.destination_y = next + step->length;
        unit->velocity.vx = 0;
        unit->velocity.vz = 3 * ONE;
        break;
    }
    unit->movement.destination_z = step->higher;
}
