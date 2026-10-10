#include "fft/battle.h"
#include "psx/types.h"

/* Clears the unit's Charging, Jump, Defending and Performing statuses (action
 * state 0), exactly as battle_status_disable_acting does. */
void battle_status_enable_disable_acting(battle_stats_t* unit) {
    update_unit_action_statuses(unit, 0);
}
