#include "fft/battle_action.h"

/* Formula 0x5A: status, 100%; dragons only (formula_check_dragon). */
void formula_dragon_hit_100(void) {
    formula_check_dragon();
    if (g_action_target_data->hit != 0) {
        formula_apply_status_to_action();
    }
}
