#include "fft/battle_action.h"
#include "psx/types.h"

/* Roll the evades (formula_calculate_final_hit_percent). Returns 1 when
 * the attack was evaded. */
s32 formula_calculate_hit(void) {
    formula_calculate_final_hit_percent();
    return g_action_target_data->hit == 0;
}
