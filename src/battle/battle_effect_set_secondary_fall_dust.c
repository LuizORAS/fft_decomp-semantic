#include "fft/battle_effect.h"

/* Raise the dust of a unit landing on hard ground, a floor or a structure
 * (battle_effect_set_landing_secondary_by_surface_type). */
void battle_effect_set_secondary_fall_dust(battle_unit_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_FALL_DUST);
}
