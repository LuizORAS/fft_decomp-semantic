#include "fft/battle_effect.h"

/* Raise the death smoke over a chocobo or a monster that has just died
 * (battle_unit_update_animation_for_status_changes). */
void battle_effect_set_secondary_death_smoke(battle_unit_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_DEATH_SMOKE);
}
