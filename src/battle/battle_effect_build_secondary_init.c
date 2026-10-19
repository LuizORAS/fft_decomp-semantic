#include "fft/battle_effect.h"

/* Fill an init record for a secondary effect that one unit both casts and
 * receives: the caster and the single target are the unit. Every other field of
 * the record is left as the caller's stack held it. */
void battle_effect_build_secondary_init(battle_unit_t* unit, battle_effect_secondary_init_t* effect) {
    effect->targets[BATTLE_EFFECT_CASTER_ENTRY].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect->targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.runtime_id = unit->runtime_id;
    effect->target_count = 1;
    effect->targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect->targets[0].data.unit.runtime_id = unit->runtime_id;
}
