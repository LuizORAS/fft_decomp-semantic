#include "fft/battle.h"
#include "psx/types.h"

/* Formula 0x15, Return 2: the magical evade check and hit chance (MA + X with both Faiths, no
 * Strengthen), then the target's CT drops to 0. */
void battle_formula_set_ct_zero(void) {
    if (battle_formula_calculate_magical_evade() == 0) {
        if (battle_formula_calculate_magic_accuracy_without_strengthen() == 0) {
            g_battle_action_target_data->ct_change = BATTLE_ACTION_CT_CHANGE_ZERO;
            g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
        }
    }
}
