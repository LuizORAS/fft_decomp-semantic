#include "fft/battle_unit.h"

/* Clear BATTLE_MOTION_FLAG_SUPPRESS_FACING_UPDATE on the unit with this runtime id: the event script's unit
 * animation instructions (script_unit_anim, script_unit_animation_rotate_event_instruction)
 * release the facing they held with battle_unit_set_facing_update_suppression. */
void battle_unit_clear_facing_update_suppression(u32 runtime_id) {
    battle_unit_t* unit = battle_unit_get_by_runtime_id(runtime_id & 0xffff);

    if (unit != 0) {
        unit->movement.suppress_facing_update = 0;
    }
}
