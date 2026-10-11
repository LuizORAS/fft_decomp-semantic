#include "fft/battle.h"
#include "psx/types.h"

/* Request an ability's effect with misc unit 0 as the caster and misc unit 1 as
 * its single target, hit normally. Nothing in the game calls it. */
void battle_effect_start_basic_ability_animation(s32 ability_id) {
    battle_effect_secondary_init_t effect_data;

    effect_data.target_count = 1;
    effect_data.targets.by_role.caster.fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect_data.targets.by_role.caster.fields.caster_id = 0;
    effect_data.targets.by_role.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect_data.targets.by_role.target[0].fields.target_id = 1;
    effect_data.targets.by_role.target[0].fields.result_animation = BATTLE_EFFECT_HIT_NORMAL;
    effect_data.spectator_count = 0;
    battle_effect_set_ability_animation(0, ability_id, &effect_data);
}
