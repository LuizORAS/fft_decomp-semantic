#include "fft/battle_move.h"

#include "fft/battle.h"
/* The 32-bit value at path holds the count in its low byte and the first path step
 * (battle_move_step_bits_e) in the next. */
#define FIRST_STEP_BITS(bits) ((bits) << 8)

/* Set up a knocked-back unit's move: a one-step path toward movement.destination (its layer in
 * BATTLE_MOVE_STEP_HIGH_LEVEL, the direction from battle_move_get_direction, no climb), the unit's
 * effective movement flags, and walk_speed 0x8000 (8.0), which battle_move_update_knockback_step slows
 * every frame. The first step is edited through the 32-bit value that starts at path (the count byte, then
 * the first step in bits 8-15), as the original does. */
void battle_move_init_knockback(battle_unit_t* unit) {
    u32* path_word = (u32*)&unit->path;
    u32 state;
    u32 knockback_state;
    s32 direction;

    unit->movement_path_offset = 0;
    unit->path.count = 1;
    unit->path.flags = battle_move_get_effective_flags(unit->stats);

    state = *path_word;
    state &= ~FIRST_STEP_BITS(BATTLE_MOVE_STEP_DISTANCE_MASK);
    state &= ~FIRST_STEP_BITS(BATTLE_MOVE_STEP_SOURCE_CLIMB);
    state &= ~FIRST_STEP_BITS(BATTLE_MOVE_STEP_DESTINATION_CLIMB);
    state &= ~FIRST_STEP_BITS(BATTLE_MOVE_STEP_ON_UNIT);
    *path_word = state;

    knockback_state = state & ~FIRST_STEP_BITS(BATTLE_MOVE_STEP_HIGH_LEVEL);
    /* The destination layer into BATTLE_MOVE_STEP_HIGH_LEVEL (bit 5 of the step). */
    knockback_state |= (unit->movement.destination_z & 1) << 13;
    *path_word = knockback_state;
    direction = battle_move_get_direction(unit);

    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        *path_word |= FIRST_STEP_BITS(3 << BATTLE_MOVE_STEP_DIRECTION_SHIFT);
        break;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        *path_word = (*path_word & ~FIRST_STEP_BITS(3 << BATTLE_MOVE_STEP_DIRECTION_SHIFT))
            | FIRST_STEP_BITS(2 << BATTLE_MOVE_STEP_DIRECTION_SHIFT);
        break;
    case BATTLE_MOVE_DIRECTION_WEST:
        *path_word = (*path_word & ~FIRST_STEP_BITS(3 << BATTLE_MOVE_STEP_DIRECTION_SHIFT))
            | FIRST_STEP_BITS(1 << BATTLE_MOVE_STEP_DIRECTION_SHIFT);
        break;
    case BATTLE_MOVE_DIRECTION_EAST:
        *path_word &= ~FIRST_STEP_BITS(3 << BATTLE_MOVE_STEP_DIRECTION_SHIFT);
        break;
    }

    unit->walk_speed = 0x8000;
}
