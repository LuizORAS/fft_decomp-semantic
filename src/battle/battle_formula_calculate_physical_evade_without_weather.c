#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_formula_calculate_physical_evade, without the weather and night penalty on bows;
 * only formula 0x05 (battle_formula_weapon_damage_without_element) uses it, and no retail weapon,
 * ability or item selects that formula. */
s32 battle_formula_calculate_physical_evade_without_weather(void) {
    battle_formula_store_physical_evade_values();
    battle_formula_calculate_concentrate();
    battle_formula_calculate_dark_confuse();
    battle_formula_calculate_abandon();
    battle_formula_apply_evasion_changes_due_to_statuses();
    battle_formula_calculate_facing_evade();
    return battle_formula_roll_evades();
}
