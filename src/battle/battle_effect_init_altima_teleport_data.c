#include "fft/battle.h"
#include "psx/types.h"

/* Fill the init record of a teleport effect: the unit is the caster and the only
 * target, and there are no spectators. Both teleport starters use it for every
 * unit, not only for Altima. */
void battle_effect_init_altima_teleport_data(
    battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* teleport_data) {
    u8 misc_id;
    u8 repeated_misc_id;

    teleport_data->targets[BATTLE_EFFECT_CASTER_ENTRY].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    misc_id = unit->unit_id;
    teleport_data->target_count = 1;
    teleport_data->targets[0].target_type = BATTLE_EFFECT_TARGET_TYPE_UNIT;
    teleport_data->targets[BATTLE_EFFECT_CASTER_ENTRY].id.unit.misc_id = misc_id;
    repeated_misc_id = unit->unit_id;
    teleport_data->spectator_count = 0;
    teleport_data->targets[0].id.unit.misc_id = repeated_misc_id;
}
