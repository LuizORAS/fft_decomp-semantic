#include "fft/battle_action.h"
#include "fft/battle_status.h"
#include "psx/types.h"

/* For an ability whose status set cancels (BATTLE_STATUS_INFLICTION_TYPE_CANCEL), copy the set to
 * g_battle_action_canceled_statuses; battle_action_run_pre_formula_setup calls it once the set is
 * loaded. */
void battle_status_store_ability_cancellations(void) {
    s32 i;
    if (g_battle_action_current_ability.status_infliction.type & BATTLE_STATUS_INFLICTION_TYPE_CANCEL) {
        for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
            g_battle_action_canceled_statuses[i] = g_battle_action_current_ability.status_infliction.statuses[i];
        }
    }
}
