#include "fft/battle_effect.h"
#include "psx/types.h"

/* Fill the effect record of a teleport effect: one target, the unit, which is also the caster, and no
 * other units. Both teleport starters fill it for every unit before they single out Altima's two
 * forms. */
void battle_effect_init_teleport_data(battle_unit_t* unit, battle_effect_secondary_init_t* teleport_data) {
    u8 runtime_id;
    u8 repeated_runtime_id;

    teleport_data->targets[BATTLE_EFFECT_CASTER_ENTRY].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    runtime_id = unit->runtime_id;
    teleport_data->target_count = 1;
    teleport_data->targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    teleport_data->targets[BATTLE_EFFECT_CASTER_ENTRY].data.unit.runtime_id = runtime_id;
    repeated_runtime_id = unit->runtime_id;
    teleport_data->spectator_count = 0;
    teleport_data->targets[0].data.unit.runtime_id = repeated_runtime_id;
}
