#include "fft/battle_action.h"
#include "psx/types.h"

/* A Dark or Confused attacker halves the base hit, which doubles every evade.
 * Then the Transparent step. */
void battle_formula_calculate_dark_confuse(void) {
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_DARKNESS)]
        & (BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_DARKNESS) | BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_CONFUSION))) {
        u8* base_hit = &g_battle_action_current_ability.base_hit;
        *base_hit = *base_hit >> 1;
    }
    battle_formula_calculate_transparent();
}
