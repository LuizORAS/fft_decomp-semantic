#include "fft/battle.h"
#include "psx/types.h"

/* Roll the evades (battle_formula_calculate_final_hit_percent). Returns 1 when
 * the attack was evaded. */
s32 battle_formula_calculate_hit(void) {
    battle_formula_calculate_final_hit_percent();
    return g_battle_action_target_data->hit == 0;
}
