#include "fft/battle_effect.h"

/* Starts the weapon-hit secondary effect of the unit's action, in the element
 * of the used weapon; is_bow is 1 for the bow hardcoding path and 0
 * otherwise, and unused. */
void battle_effect_set_evade_type_data_and_weapon_element(battle_unit_misc_data_t* unit, s32 is_bow) {
    battle_effect_secondary_init_t secondary_init;
    battle_effect_build_secondary_init_from_action(unit, &secondary_init);
    battle_effect_init_weapon_hit_secondary(
        g_main_item_weapon_data[unit->used_item_or_weapon_id].element, &secondary_init, is_bow);
}
