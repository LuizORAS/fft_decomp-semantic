#include "fft/battle_effect.h"

/* Show the zodiac poof on a unit that has just become a Crystal or a Treasure
 * (battle_unit_update_animation_for_status_changes). */
void battle_effect_set_secondary_zodiac_poof(battle_unit_t* unit) {
    battle_effect_set_secondary(unit, BATTLE_EFFECT_SECONDARY_ANIMATION_ZODIAC_POOF);
}
