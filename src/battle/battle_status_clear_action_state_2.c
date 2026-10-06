#include "fft/battle.h"
#include "psx/types.h"

/* Same as battle_status_clear_action_state; only battle_status_check_charging_charge calls it. */
void battle_status_clear_action_state_2(battle_stats_t* unit) {
    main_status_set_action_state(unit, MAIN_UNIT_ACTION_STATE_NONE);
}
