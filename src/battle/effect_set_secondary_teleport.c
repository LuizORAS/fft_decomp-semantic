#include "fft/battle_effect.h"

void effect_set_secondary_teleport(battle_unit_t* unit) {
    effect_set_secondary(unit, EFFECT_SECONDARY_ANIMATION_TELEPORT);
}
