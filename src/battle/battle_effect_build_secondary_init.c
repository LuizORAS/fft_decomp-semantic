#include "fft/battle.h"

void battle_effect_build_secondary_init(battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* effect) {
    effect->caster.fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect->caster.fields.caster_id = unit->unit_id;
    effect->target_count = 1;
    effect->target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    effect->target[0].fields.target_id = unit->unit_id;
}
