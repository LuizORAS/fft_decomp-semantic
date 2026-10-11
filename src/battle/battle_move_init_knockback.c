#include "fft/battle.h"

/*
 * Prepares a knocked-back unit's movement record: a one-step path, the
 * fly/float movement flags, and the direction bits (0x4000/0x8000 of the
 * packed 0x9c 32-bit value) chosen by battle_move_get_direction.
 * The field at 0x9c is handled as a 32-bit value: it packs the path count byte with the
 * first path step in bits 8-15 (battle_move_step_t). The step's length, slow,
 * climb and mount bits are cleared and its higher bit (0x2000) takes the
 * destination layer.
 */
void battle_move_init_knockback(battle_unit_misc_data_t* unit) {
    u32* path_word = (u32*)&unit->path;
    u32 state;
    u32 knockback_state;
    s32 direction;

    unit->movement_path_offset = 0;
    unit->path.count = 1;
    unit->path.flags = battle_move_get_effective_flags(unit->battle_data);

    state = *path_word;
    state &= ~0x300;
    state &= ~0x400;
    state &= ~0x800;
    state &= ~0x1000;
    *path_word = state;

    knockback_state = state & ~0x2000;
    knockback_state |= (unit->movement.bytes.destination_z & 1) << 13;
    *path_word = knockback_state;
    direction = battle_move_get_direction(unit);

    switch (direction) {
    case BATTLE_MOVE_DIRECTION_NORTH:
        *path_word |= 0xc000;
        break;
    case BATTLE_MOVE_DIRECTION_SOUTH:
        *path_word = (*path_word & ~0xc000) | 0x8000;
        break;
    case BATTLE_MOVE_DIRECTION_WEST:
        *path_word = (*path_word & ~0xc000) | 0x4000;
        break;
    case BATTLE_MOVE_DIRECTION_EAST:
        *path_word &= ~0xc000;
        break;
    }

    unit->walk_speed = 0x8000;
}
