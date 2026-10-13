#include "fft/battle_unit.h"
#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* UnitAnim event instruction: request the animation for every unit record
 * that resolves from the unit id, and wait for the effect after a bow shot.
 * WORLD twin of script_unit_anim. */
void world_script_unit_anim(const event_unit_anim_parameters_t* parameters) {
    u16 unit_id;
    s32 filter_mode;
    s32 unit_index;
    s32 animation;

    unit_id = script_read_s16(parameters->unit_id_le);
    if (script_prepare_unit_filter(&unit_id, &filter_mode) != 0) {
        for (unit_index = 0; unit_index < BATTLE_UNIT_SLOT_COUNT; unit_index++) {
            if (world_script_filter_unit_id_by_mode(&unit_id, &unit_index, &filter_mode) != 0) {
                if (parameters->facing_update_mode == 0) {
                    battle_unit_clear_facing_update_suppression(unit_id);
                } else if (parameters->facing_update_mode == 1) {
                    battle_unit_set_facing_update_suppression(unit_id);
                }
                animation = script_read_s16(parameters->animation_le);
                unit_set_animation(unit_id, animation);
                if (animation >= BATTLE_UNIT_ANIMATION_BOW_HIGH && animation <= BATTLE_UNIT_ANIMATION_BOW_LOW) {
                    script_play_effect_and_wait();
                }
                if (filter_mode == 0) {
                    break;
                }
            }
        }
    }
}
