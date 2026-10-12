#include "fft/battle_action.h"

/* Formula 0x5D: Quick; dragons only. */
void battle_formula_dragon_set_quick(void) {
    battle_formula_check_dragon();
    if (g_action_target_data->hit != 0) {
        battle_formula_apply_quick_effect();
    }
}
