#include "fft/battle_effect.h"

void effect_set_secondary_death_smoke(battle_unit_misc_data_t* unit) {
    effect_set_secondary(unit, EFFECT_SECONDARY_ANIMATION_DEATH_SMOKE);
}
