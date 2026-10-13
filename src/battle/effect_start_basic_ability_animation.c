#include "fft/battle_effect.h"
#include "psx/types.h"

/* Request an ability's effect with runtime unit 0 as the caster and runtime unit 1 as
 * its single target, hit normally. Nothing in the game calls it. */
void effect_start_basic_ability_animation(s32 ability_id) {
    effect_secondary_init_t effect_data;

    effect_data.target_count = 1;
    effect_data.targets[EFFECT_CASTER_ENTRY].target_type = EFFECT_TARGET_TYPE_UNIT;
    effect_data.targets[EFFECT_CASTER_ENTRY].data.unit.runtime_id = 0;
    effect_data.targets[0].target_type = EFFECT_TARGET_TYPE_UNIT;
    effect_data.targets[0].data.unit.runtime_id = 1;
    effect_data.targets[0].data.unit.result_animation = EFFECT_HIT_NORMAL;
    effect_data.spectator_count = 0;
    effect_set_ability_animation(0, ability_id, &effect_data);
}
