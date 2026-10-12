#include "fft/battle_action.h"
#include "fft/battle_status.h"
#include "psx/types.h"

/* For an ability whose status infliction type is cancel, copies its status bytes
 * to g_action_canceled_statuses. Run during the pre-formula setup. */
void battle_status_remove(void) {
    s32 i;
    if (g_action_current_ability.status_infliction.type & BATTLE_STATUS_INFLICTION_TYPE_CANCEL) {
        i = 0;
        do {
            g_action_canceled_statuses[i] = g_action_current_ability.status_infliction.statuses[i];
            i++;
        } while (i < BATTLE_STATUS_BYTE_COUNT);
    }
}
