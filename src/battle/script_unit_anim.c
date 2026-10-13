#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/*
 * UnitAnim event instruction.  Parameters: 0x00 unit id
 * (16-bit value), 0x02 animation id (16-bit value), 0x04 move-flag mode (0 unset,
 * 1 flip).  A unit id above 0xff selects a group, so the unit-state loop
 * runs over all 21 unit indices.
 */
void script_unit_anim(const event_unit_anim_parameters_t* parameters) {
    u16 runtime_id;
    s32 state;
    s32 unit_index;
    s32 animation;
    s32 move_flag_mode;

    runtime_id = script_read_s16(parameters->unit_id_le);
    if (script_prepare_unit_filter(&runtime_id, &state) == 0) {
        return;
    }
    for (unit_index = 0; unit_index < EVENT_UNIT_SLOT_COUNT; unit_index++) {
        if (script_filter_unit_id_by_mode(&runtime_id, (u16*)&unit_index, &state) != 0) {
            move_flag_mode = parameters->facing_update_mode;
            if (move_flag_mode == 0) {
                battle_unit_clear_facing_update_suppression(runtime_id);
            } else if (move_flag_mode == 1) {
                battle_unit_set_facing_update_suppression(runtime_id);
            }
            animation = script_read_s16(parameters->animation_le);
            unit_set_animation(runtime_id, animation);
            if (animation >= BATTLE_UNIT_ANIMATION_BOW_HIGH && animation <= BATTLE_UNIT_ANIMATION_BOW_LOW) {
                script_play_effect_and_wait();
            }
            if (state == 0) {
                return;
            }
        }
    }
}
