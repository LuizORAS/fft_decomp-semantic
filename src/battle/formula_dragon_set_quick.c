#include "fft/battle_action.h"
#include "fft/battle_formula.h"

/* Formula 0x5D: Quick; dragons only. */
void formula_dragon_set_quick(void) {
    formula_check_dragon();
    if (g_action_target_data->hit != 0) {
        formula_apply_quick_effect();
    }
}
