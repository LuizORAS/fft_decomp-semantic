#include "fft/wldcore.h"
#include "psx/types.h"

/* Inverse of wldcore_calendar_calculate_month_and_day: 1-based month and day to a
 * 0-based day-of-year index (party_unit_t.birthday_day). */
s32 wldcore_calendar_calculate_day_of_year(s32 month, s32 day) {
    s32 index;
    s32 days_before_month;
    s32 day_of_year;

    month -= 1;
    days_before_month = 0;
    for (index = 0; index < month; index++)
        days_before_month += g_wldcore_calendar_days_per_month[index];
    day_of_year = days_before_month - 1;
    return day_of_year + day;
}
