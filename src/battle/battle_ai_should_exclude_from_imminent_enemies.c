#include "fft/battle.h"
#include "psx/types.h"

/* Return 1 when unit_id is no imminent enemy: not an enemy, acting after the acting unit's next turn,
 * or still under Stop, Don't Act, Sleep, Chicken or Charm by then. */
s32 battle_ai_should_exclude_from_imminent_enemies(s32 unit_id) {
    battle_stats_t* unit;
    s32 limit;

    if ((&g_battle_ai_unit_enemy_flag)[unit_id * 0x10] == 0)
        return 1;
    unit = &g_battle_unit_stats[unit_id];
    limit = g_battle_ai_acting_unit_remaining_clockticks;
    if (limit < battle_ai_calculate_clockticks_until_unit_acts(unit)
        || battle_ai_is_status_active_through_delay(
            g_battle_ai_acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_STOP)
        || battle_ai_is_status_active_through_delay(
            g_battle_ai_acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_DONT_ACT)
        || battle_ai_is_status_active_through_delay(
            g_battle_ai_acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_SLEEP)
        || battle_ai_is_status_active_through_delay(
            g_battle_ai_acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_CHICKEN))
        return 1;
    return battle_ai_is_status_active_through_delay(
               g_battle_ai_acting_unit_remaining_clockticks, unit, BATTLE_STATUS_ID_CHARM)
        != 0;
}
