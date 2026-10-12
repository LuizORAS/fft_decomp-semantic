#include "fft/battle_move.h"

/* Runs the knockback step unless the unit is mid-way through animation 0x20. */
void battle_move_update_knockback_after_animation(battle_unit_misc_data_t* unit) {
    if (((unit->animation.sequence_id >> 1) != BATTLE_UNIT_ANIMATION_LAND) || (unit->animation.wait == 0)) {
        battle_move_advance_display_unit_step(unit);
    }
}
