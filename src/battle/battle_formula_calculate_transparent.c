#include "fft/battle.h"
#include "psx/types.h"

void battle_formula_calculate_transparent(void) {
    /* Status 3 bit 0x10: Transparent. */
    if (g_battle_action_attacker->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_TRANSPARENT)]
        & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_TRANSPARENT)) {
        main_util_clear_byte_data(&g_battle_action_current_ability.accessory_evade, 4);
    }
}
