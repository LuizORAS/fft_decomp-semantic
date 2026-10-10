#include "fft/battle.h"
#include "psx/types.h"

/* Clear the record's status inflictions and removals, and the shared list of
 * cancelled statuses with them. */
void battle_action_clear_status_changes(battle_action_data_t* action) {
    s32 i;
    for (i = 0; i < BATTLE_STATUS_BYTE_COUNT; i++) {
        g_battle_action_canceled_statuses[i] = 0;
        action->status_infliction[i] = 0;
        action->status_removal[i] = 0;
    }
}
