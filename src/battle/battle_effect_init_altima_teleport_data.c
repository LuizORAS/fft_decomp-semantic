#include "fft/battle.h"
#include "psx/types.h"

void battle_effect_init_altima_teleport_data(
    battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* teleport_data) {
    u8 misc_id;
    u8 repeated_misc_id;

    teleport_data->caster.fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    misc_id = unit->unit_id;
    teleport_data->target_count = 1;
    teleport_data->target[0].fields.target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    teleport_data->caster.fields.caster_id = misc_id;
    repeated_misc_id = unit->unit_id;
    teleport_data->spectator_count = 0;
    teleport_data->target[0].fields.target_id = repeated_misc_id;
}
