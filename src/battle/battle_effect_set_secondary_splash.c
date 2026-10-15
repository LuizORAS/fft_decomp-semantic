#include "fft/battle.h"

/* Raise the splash of a unit landing in swamp, marsh or water
 * (battle_effect_set_landing_secondary_by_surface_type). */
void battle_effect_set_secondary_splash(battle_unit_misc_data_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_SPLASH);
}
