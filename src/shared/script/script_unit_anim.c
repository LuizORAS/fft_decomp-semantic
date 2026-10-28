#include "fft/battle_script.h"
#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/* UnitAnim (EVENT_OPCODE_UNIT_ANIM): start an animation on the unit, or on each unit of a group,
 * after setting or clearing its facing-update suppression (the byte at +4: 0 clears it, 1 sets it,
 * other values leave it). After a bow animation it plays the staged effect and waits for it
 * (script_play_effect_and_wait). BATTLE and WORLD compile this text.
 *
 * A unit id above 0xff selects a group, so the runtime-state loop runs over all 21 unit indices. */
void script_unit_anim(const event_unit_anim_parameters_t* parameters) {
    u16 runtime_id;
    s32 state;
    s32 unit_index;
    s32 animation;
    s32 facing_suppression;

    runtime_id = script_load_halfword(parameters->unit_id_le);
    if (script_prepare_unit_filter(&runtime_id, &state) == 0) {
        return;
    }
    for (unit_index = 0; unit_index < EVENT_UNIT_SLOT_COUNT; unit_index++) {
        if (script_filter_unit_id_by_mode(&runtime_id, (u16*)&unit_index, &state) != 0) {
            facing_suppression = parameters->facing_update_mode;
            if (facing_suppression == 0) {
                battle_unit_clear_facing_update_suppression(runtime_id);
            } else if (facing_suppression == 1) {
                battle_unit_set_facing_update_suppression(runtime_id);
            }
            animation = script_load_halfword(parameters->animation_le);
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
