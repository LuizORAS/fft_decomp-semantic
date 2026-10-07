#include "fft/battle.h"
#include "psx/types.h"

/* XA * YA as HP damage, then the weather (battle_formula_apply_weather_elemental_effects) and the
 * ability's element (battle_formula_apply_ability_element). Returns 1 when the element nullified the
 * hit, else 0. */
s32 battle_formula_calculate_elemental_xa_times_ya(void) {
    battle_formula_store_xa_times_ya_damage();
    battle_formula_apply_weather_elemental_effects();
    battle_formula_apply_ability_element();
    return g_battle_action_target_data->hit == 0;
}
