#include "fft/battle.h"
#include "psx/types.h"

/* Clears the unit's Charging, Jump, Defending and Performing statuses (action
 * state 0). update_unit_action_statuses is BATTLE's link name for
 * main_status_set_action_state. */
void battle_status_disable_acting(battle_stats_t* unit) {
    update_unit_action_statuses(unit, 0);
}
