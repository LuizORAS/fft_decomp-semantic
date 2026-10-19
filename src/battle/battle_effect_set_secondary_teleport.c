#include "fft/battle_effect.h"

/* Show the teleport effect on a unit: battle_effect_start_teleport_departure and
 * battle_effect_start_teleport_arrival start it for every unit but Altima's two forms, and
 * battle_effect_set_secondary_teleport_by_runtime_id for the event instructions. */
void battle_effect_set_secondary_teleport(battle_unit_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_TELEPORT);
}
