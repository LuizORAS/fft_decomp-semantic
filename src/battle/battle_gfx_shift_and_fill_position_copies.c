#include "fft/battle.h"

/* Advances the unit's position trail: each copy takes the next newer one and copy
 * 0 the unit's current position. */
void battle_gfx_shift_and_fill_position_copies(battle_unit_misc_data_t* unit) {
    s32 index;

    index = 5;
    do {
        unit->position_copies[index] = unit->position_copies[index - 1];
        index--;
    } while (index > 0);
    battle_unit_copy_misc_data_xyz_values(unit->position_copies, unit);
}
