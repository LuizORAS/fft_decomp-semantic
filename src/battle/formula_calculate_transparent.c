#include "fft/battle_action.h"
#include "fft/battle_formula.h"
#include "fft/memory.h"

/* A Transparent attacker cannot be evaded: the four evades become 0. */
void formula_calculate_transparent(void) {
    /* Status 3 bit 0x10: Transparent. */
    if (g_action_attacker->status_sets.current[STATUS_BYTE_INDEX(STATUS_ID_TRANSPARENT)]
        & STATUS_BYTE_MASK(STATUS_ID_TRANSPARENT)) {
        main_util_clear_byte_data(&g_action_current_ability.accessory_evade, 4);
    }
}
