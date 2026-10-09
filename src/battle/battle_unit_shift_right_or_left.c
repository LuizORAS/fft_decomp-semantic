#include "fft/battle.h"

void battle_unit_shift_right_or_left(battle_unit_misc_data_t* unit, s8 delta) {
    s16* vector;
    switch (unit->facing & 0xC00) {
    case 0x800:
        vector = &unit->seq_offset.vx;
        *vector = *vector + delta;
        return;
    case 0x0:
        vector = &unit->seq_offset.vx;
        *vector = *vector - delta;
        return;
    case 0x400:
        vector = &unit->seq_offset.vz;
        *vector = *vector + delta;
        return;
    case 0xC00:
        vector = &unit->seq_offset.vz;
        *vector = *vector - delta;
        return;
    }
}
