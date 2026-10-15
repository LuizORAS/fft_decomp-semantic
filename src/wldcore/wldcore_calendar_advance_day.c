#include "fft/wldcore.h"

/* Advance the calendar one day, as the marker reaches each location it travels through: the day and
 * month variables roll over by g_wldcore_days_per_month (no leap years), and March 21, the first day
 * of Aries, adds a year to EVENT_SCRIPT_VAR_YEAR_COUNT (capped at 99). Then the daily updates run:
 * the propositions' day counters (wldcore_proposition_advance_daily_counters), the birthdays
 * (wldcore_calendar_advance_birthdays) and the monster breeding (world_unit_update_monster_breeding).
 *
 * The target does not validate the incoming month or day. */
void wldcore_calendar_advance_day(void) {
    s32 month = world_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
    s32 day = world_script_get_variable(EVENT_SCRIPT_VAR_DAY) + 1;

    if (g_wldcore_days_per_month[month - 1] < day) {
        month++;
        if (month == 13) {
            month = 1;
        }
        day = 1;
    }
    world_script_set_variable(EVENT_SCRIPT_VAR_MONTH, month);
    world_script_set_variable(EVENT_SCRIPT_VAR_DAY, day);
    if (month == 3 && day == 21) {
        s32 value = world_script_get_variable(EVENT_SCRIPT_VAR_YEAR_COUNT) + 1;
        if (value >= 100) {
            value = 99;
        }
        world_script_set_variable(EVENT_SCRIPT_VAR_YEAR_COUNT, value);
    }
    wldcore_proposition_advance_daily_counters();
    wldcore_calendar_advance_birthdays();
    world_unit_update_monster_breeding(1);
}
