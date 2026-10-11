#include "fft/battle.h"
#include "psx/types.h"

/* Start a unit's jump step toward the next path tile.
 *
 * The step byte's low two bits give the jump length in tiles and its top two
 * bits the direction. The vertical velocity scales with the length; a long
 * jump (length 2+) uses animation 0x1e with a per-direction facing, while a
 * short hop dismounts any rider and toggles movement bit 27 to alternate the
 * 0x30/0x31 hop animation. */
void battle_move_start_unit_jump_step(
    battle_unit_misc_data_t* unit, const map_tile_t* unused_from, const map_tile_t* unused_to) {
    u16 facings[4] = { BATTLE_FACING_EAST, BATTLE_FACING_WEST, BATTLE_FACING_SOUTH, BATTLE_FACING_NORTH };
    u8 next;
    u8 jump_phases[4] = { BATTLE_MOVE_STEP_PHASE_JUMP_START_EAST, BATTLE_MOVE_STEP_PHASE_JUMP_START_WEST,
        BATTLE_MOVE_STEP_PHASE_JUMP_START_SOUTH, BATTLE_MOVE_STEP_PHASE_JUMP_START_NORTH };
    u8 hop_phases[4] = { BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_EAST, BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_WEST,
        BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_SOUTH, BATTLE_MOVE_STEP_PHASE_JUMP_ASCENT_NORTH };
    battle_move_step_t* step;
    s32 animation;
    battle_move_step_t current_step;

    /* Copy the byte to retain narrow loads of the packed step fields. */
    current_step = unit->current_path_step;
    unit->velocity.vy = -g_battle_move_jump_gravity * (((current_step.bits.length * 0x1C000) + 0xE000) / 24576);
    step = &unit->current_path_step;
    current_step = unit->current_path_step;
    if (current_step.bits.length >= 2) {
        unit->step_phase = jump_phases[current_step.bits.direction];
        current_step = unit->current_path_step;
        battle_unit_store_animation_facing(BATTLE_UNIT_ANIMATION_JUMP, facings[current_step.bits.direction], unit);
    } else {
        unit->step_phase = hop_phases[current_step.bits.direction];
        battle_unit_dismount_rider(unit);
        animation = BATTLE_UNIT_ANIMATION_HOP;
        if (unit->movement.word & BATTLE_MOTION_FLAG_ALTERNATE_SHORT_HOP) {
            animation = BATTLE_UNIT_ANIMATION_HOP_ALTERNATE;
        }
        battle_unit_store_animation_facing(animation, unit->facing, unit);
        unit->movement.flags.alternate_short_hop ^= 1;
    }
    switch (step->bits.direction) {
    case BATTLE_MOVE_STEP_DIRECTION_EAST:
        next = unit->map_x + 1;
        unit->movement.bytes.destination_x = next + step->bits.length;
        unit->velocity.vx = 0x3000;
        unit->velocity.vz = 0;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_WEST:
        next = unit->map_x - 1;
        unit->movement.bytes.destination_x = next - step->bits.length;
        unit->velocity.vx = -0x3000;
        unit->velocity.vz = 0;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_SOUTH:
        next = unit->map_y - 1;
        unit->movement.bytes.destination_y = next - step->bits.length;
        unit->velocity.vx = 0;
        unit->velocity.vz = -0x3000;
        break;
    case BATTLE_MOVE_STEP_DIRECTION_NORTH:
        next = unit->map_y + 1;
        unit->movement.bytes.destination_y = next + step->bits.length;
        unit->velocity.vx = 0;
        unit->velocity.vz = 0x3000;
        break;
    }
    unit->movement.bytes.destination_z = step->bits.higher;
}
