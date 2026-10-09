#include "fft/battle.h"

void battle_unit_shift_right_or_left(battle_unit_misc_data_t* unit, s8 delta) {
    s16* vector;
    switch (unit->facing & 0xC00) {
    case BATTLE_FACING_NORTH:
        vector = &unit->seq_offset.vx;
        *vector = *vector + delta;
        return;
    case BATTLE_FACING_SOUTH:
        vector = &unit->seq_offset.vx;
        *vector = *vector - delta;
        return;
    case BATTLE_FACING_WEST:
        vector = &unit->seq_offset.vz;
        *vector = *vector + delta;
        return;
    case BATTLE_FACING_EAST:
        vector = &unit->seq_offset.vz;
        *vector = *vector - delta;
        return;
    }
}
