#include "fft/battle.h"
#include "psx/types.h"

/* UnitAnim (EVENT_OPCODE_UNIT_ANIM): start an animation on the unit, or on each unit of a group,
 * after setting or clearing its facing-update suppression (the byte at +4: 0 clears it, 1 sets it,
 * other values leave it). After a bow animation it plays the staged effect and waits for it
 * (battle_script_play_effect_and_wait).
 *
 * A unit id above 0xff selects a group, so the misc-state loop runs over all 21 unit indices. */
void battle_script_unit_anim(u8* parameters) {
    u16 misc_id;
    s32 state;
    s32 unit_index;
    s32 animation;
    s32 facing_suppression;

    misc_id = battle_script_load_halfword(parameters);
    if (battle_unit_try_get_misc_data_by_unit_id(&misc_id, &state) == 0) {
        return;
    }
    unit_index = 0;
    do {
        if (battle_script_filter_unit_id_by_mode(&misc_id, (u16*)&unit_index, &state) != 0) {
            facing_suppression = parameters[4];
            if (facing_suppression == 0) {
                battle_unit_clear_facing_update_suppression(misc_id);
            } else if (facing_suppression == 1) {
                battle_unit_set_facing_update_suppression(misc_id);
            }
            animation = battle_script_load_halfword(parameters + 2);
            battle_unit_set_animation(misc_id, animation);
            if (animation >= BATTLE_UNIT_ANIMATION_BOW_HIGH && animation <= BATTLE_UNIT_ANIMATION_BOW_LOW) {
                battle_script_play_effect_and_wait();
            }
            if (state == 0) {
                return;
            }
        }
        unit_index = unit_index + 1;
    } while (unit_index < EVENT_UNIT_SLOT_COUNT);
}
