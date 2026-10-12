#include "fft/battle_ai.h"
#include "fft/battle_unit.h"
#include "psx/types.h"

/* Rejects a unit from the AI's imminent-enemy set.
 *
 * The unit must be an enemy that acts no later than the acting unit and will
 * no longer be disabled by Stop, Don't Act, Sleep, Chicken, or Charm then. */
s32 ai_should_exclude_from_imminent_enemies(s32 unit_id) {
    ai_data_t* ai = &g_ai_data_base;
    battle_stats_t* unit;
    s32 limit;

    if (g_ai_data_base.unit_decisions[unit_id].enemy_flag == 0)
        return 1;
    unit = &g_battle_unit_stats[unit_id];
    limit = ai->acting_unit_remaining_clockticks;
    if (limit < ai_calculate_clockticks_until_unit_acts(unit)
        || ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, STATUS_ID_STOP)
        || ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, STATUS_ID_DONT_ACT)
        || ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, STATUS_ID_SLEEP)
        || ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, STATUS_ID_CHICKEN))
        return 1;
    return ai_is_status_active_through_delay(ai->acting_unit_remaining_clockticks, unit, STATUS_ID_CHARM) != 0;
}
