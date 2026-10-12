#include "fft/battle_action.h"

/* A Transparent attacker cannot be evaded: the four evades become 0. */
void battle_formula_calculate_transparent(void) {
    /* Status 3 bit 0x10: Transparent. */
    if (g_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT)) {
        main_util_clear_byte_data(&g_action_current_ability.accessory_evade, 4);
    }
}
