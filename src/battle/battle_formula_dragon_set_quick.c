#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x5D, Dragon Level Up: only on a dragon or a hydra (battle_formula_check_dragon); then
 * Quick (battle_formula_apply_quick_effect), not a level. */
void battle_formula_dragon_set_quick(void) {
    battle_formula_check_dragon();
    if (g_battle_action_target_data->hit != 0) {
        battle_formula_apply_quick_effect();
    }
}
