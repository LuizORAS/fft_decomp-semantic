#include "fft/battle.h"
#include "psx/types.h"

/* Run battle_formula_calculate_final_hit_percent (the shown accuracy and the evade rolls, only for
 * the basic attack and abilities flagged evadeable); returns 1 when the target evaded. */
s32 battle_formula_roll_evades(void) {
    battle_formula_calculate_final_hit_percent();
    return g_battle_action_target_data->hit == 0;
}
