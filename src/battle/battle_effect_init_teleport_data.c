#include "fft/battle.h"
#include "psx/types.h"

/* Fill the effect record of a teleport effect: one target, with the unit as both caster and target,
 * and no palette targets. */
void battle_effect_init_teleport_data(battle_unit_misc_data_t* unit, battle_effect_secondary_init_t* teleport_data) {
    u8 misc_id;
    u8 repeated_misc_id;

    teleport_data->caster.fields.target_type = 0;
    misc_id = unit->unit_id;
    teleport_data->target_count = 1;
    teleport_data->target.fields.target_type = 0;
    teleport_data->caster.fields.caster_id = misc_id;
    repeated_misc_id = unit->unit_id;
    teleport_data->palette_target_count = 0;
    teleport_data->target.fields.target_id = repeated_misc_id;
}
