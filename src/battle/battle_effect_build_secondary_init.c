#include "fft/battle.h"

/* Fill an init record for a secondary effect that one unit both casts and
 * receives: the caster and the single target are the unit. Every other field of
 * the record is left as the caller's stack held it. */
void battle_effect_build_secondary_init(battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* effect) {
    effect->targets.by_role.caster.fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect->targets.by_role.caster.fields.caster_id = unit->unit_id;
    effect->target_count = 1;
    effect->targets.by_role.target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect->targets.by_role.target[0].fields.target_id = unit->unit_id;
}
