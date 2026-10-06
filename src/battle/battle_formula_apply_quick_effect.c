#include "fft/battle.h"
#include "psx/types.h"

/* Give the target Quick (CT change 0xFF) as a pseudo-status result: the Quick spell, and Dragon
 * Level Up, whose formula (0x5d) gives a dragon or a hydra Quick. */
void battle_formula_apply_quick_effect(void) {
    g_battle_action_target_data->ct_change = 0xFF;
    g_battle_action_target_data->attack_type = BATTLE_ACTION_TYPE_PSEUDO_STATUS;
}
