#include "fft/wldcore.h"
#include "psx/types.h"

/* Convert a calendar date in place into a zodiac sign (1 = Aries, from March 21) and the day within
 * that sign, by g_wldcore_calendar_zodiac_start_dates. An out-of-range month or day plays MAIN_SFX_INVALID
 * and becomes 1/1.
 *
 * The second table pointer for the previous sign's start month keeps the loop
 * base pointer's reference count low enough for global allocation to give the
 * loop-hoisted month copy the lower register, as the target does. */
void wldcore_calendar_to_zodiac_date(s32* month_ptr, s32* day_ptr) {
    s32 index;
    s32 delta;
    s32 previous_month;
    calendar_date_t* start_dates;
    u8* start_days;
    calendar_date_t* previous_dates;

    index = 0;
    if (*month_ptr < 1 || *month_ptr > 12 || *day_ptr < 1 || *day_ptr > 31) {
        wldcore_sound_play_effect(MAIN_SFX_INVALID);
        *day_ptr = 1;
        *month_ptr = 1;
        return;
    }
    start_dates = g_wldcore_calendar_zodiac_start_dates;
    while (start_dates[index].month != *month_ptr) {
        index = (index + 1) % 12;
    }
    /* Direct .day indexing folds the two original indexed addresses into one. */
    start_days = &g_wldcore_calendar_zodiac_start_dates[0].day;
    if (*day_ptr < start_days[index * sizeof(calendar_date_t)]) {
        index = (index + 11) % 12;
        *month_ptr = index + 1;
        /* Separate month-table local preserves the original register allocation. */
        previous_dates = g_wldcore_calendar_zodiac_start_dates;
        previous_month = previous_dates[index].month;
        delta = start_days[index * sizeof(calendar_date_t)];
        delta = g_wldcore_calendar_days_per_month[previous_month - 1] - delta + 1;
        *day_ptr += delta;
    } else {
        *month_ptr = index + 1;
        *day_ptr = *day_ptr - start_days[index * sizeof(calendar_date_t)] + 1;
    }
}
