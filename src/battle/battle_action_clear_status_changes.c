#include "fft/battle_action.h"
#include "psx/types.h"

/* Clear a result record's status inflictions and removals, and the ability's canceled-status list
 * (g_battle_action_canceled_statuses). */
void battle_action_clear_status_changes(battle_action_data_t* action) {
    s32 i;
    for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
        g_battle_action_canceled_statuses[i] = 0;
        action->status_changes.infliction[i] = 0;
        action->status_changes.removal[i] = 0;
    }
}
