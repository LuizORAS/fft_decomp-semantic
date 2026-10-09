#include "fft/battle.h"
#include "psx/types.h"

/* Rejects a unit from the AI's imminent-enemy set.
 *
 * The unit must be an enemy that acts no later than the acting unit and will
 * no longer be disabled by Stop, Don't Act, Sleep, Chicken, or Charm then. */
s32 battle_ai_should_exclude_from_imminent_enemies(s32 unit_id) {
    battle_ai_data_t* ai = &g_battle_ai_data_base;
    battle_stats_t* unit;
    s32 limit;

    if (g_battle_ai_data_base.unit_decisions[unit_id].enemy_flag == 0)
        return 1;
    unit = &g_battle_unit_stats[unit_id];
    limit = ai->acting_unit_remaining_clockticks;
    if (limit < battle_ai_calculate_clockticks_until_unit_acts(unit)
        || battle_ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_STOP)
        || battle_ai_is_status_active_through_delay(
            ai->acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_DONT_ACT)
        || battle_ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_SLEEP)
        || battle_ai_is_status_active_through_delay(
            ai->acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_CHICKEN))
        return 1;
    return battle_ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_CHARM)
        != 0;
}
