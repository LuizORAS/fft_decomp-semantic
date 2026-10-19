#include "fft/battle_status.h"
#include "fft/main.h"
#include "psx/types.h"

/* Set the unit's action state to Defending (main_status_set_action_state): the Defend command from
 * the menus, the AI and its simulation. Returns 0. */
s32 battle_status_inflict_defending_to_battle_id(s32 unit_id) {
    main_status_set_action_state(
        &g_battle_unit_stats[unit_id], BATTLE_STATUS_HANDLER_INDEX(BATTLE_STATUS_ID_DEFENDING));
    return 0;
}
