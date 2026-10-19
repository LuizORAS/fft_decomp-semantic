#include "fft/wldcore.h"
#include "psx/types.h"

/* A 0-based day of the year (January 1 being 0, as party_unit_t.birthday_day keeps it) to a 1-based
 * month and day. */
void wldcore_calendar_from_day_of_year(s32 day_of_year, s32* output_month, s32* output_day) {
    s32 month;
    s32 days_in_month;

    month = 0;
    while (1) {
        days_in_month = g_wldcore_calendar_days_per_month[month];
        if (day_of_year < days_in_month) {
            break;
        }
        month += 1;
        day_of_year -= days_in_month;
        if (month >= 12) {
            break;
        }
    }
    *output_month = month + 1;
    *output_day = day_of_year + 1;
}
