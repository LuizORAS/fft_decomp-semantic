#include "fft/battle_action.h"
#include "psx/types.h"

/* A Dark or Confused attacker halves the base hit, which doubles every evade.
 * Then the Transparent step. */
void formula_calculate_dark_confuse(void) {
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_DARKNESS)]
        & (STATUS_BYTE_MASK(STATUS_ID_DARKNESS) | STATUS_BYTE_MASK(STATUS_ID_CONFUSION))) {
        u8* base_hit = &g_action_current_ability.base_hit;
        *base_hit = *base_hit >> 1;
    }
    formula_calculate_transparent();
}
