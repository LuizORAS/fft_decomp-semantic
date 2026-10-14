#include "fft/battle.h"

/* Clear BATTLE_MOTION_FLAG_SUPPRESS_FACING_UPDATE on the unit with this misc id: the event script's unit
 * animation instructions (battle_script_unit_anim, battle_script_unit_animation_rotate_event_instruction)
 * release the facing they held with battle_unit_set_facing_update_suppression. */
void battle_unit_clear_facing_update_suppression(u32 misc_id) {
    battle_unit_misc_data_t* unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);

    if (unit != 0) {
        unit->movement.word &= ~BATTLE_MOTION_FLAG_SUPPRESS_FACING_UPDATE;
    }
}
