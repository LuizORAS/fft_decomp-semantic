#include "fft/wldcore.h"

/* Advance the calendar by the days an event asked for (EVENT_SCRIPT_VAR_DATE_ADVANCE, applied as a
 * visit starts by wldcore_init_world_menus_and_date). Whole years add to EVENT_SCRIPT_VAR_YEAR_COUNT
 * and to every Brave Story age; the remaining days move the date forward and add one more year when
 * they cross December 31. A character whose birthday falls within the remaining days, later in the
 * same calendar year, gains a year; ages are capped at 99. Entry 0 is Ramza, whose birthday lives in
 * EVENT_SCRIPT_VAR_PLAYER_BIRTH_MONTH and _DAY, as in wldcore_calendar_advance_birthdays.
 *
 * An active proposition returns (WLDCORE_PROPOSITION_PROGRESS_RETURNED) when its elapsed days plus the
 * advance reach its assigned days (elapsed is set to them), or with one more elapsed day when it was
 * recalled (WLDCORE_PROPOSITION_PROGRESS_RECALLED); the others keep their elapsed count. Nonzero day
 * counts in the proposition states drop by the advance, floored at 0, keeping the failure bits. See
 * QUIRKS.md for how this differs from the daily advance.
 *
 * The birthday table's element type must not be const: a const element makes
 * the reads unchanging, and the scheduler then hoists the second table read
 * above the store of the first. The loop's `i = 0` must sit at the loop, not
 * at function entry, for the first call's `li a0,0x67` to precede the
 * reciprocal-division `mult`. */
void wldcore_calendar_advance_days(s32 days) {
    s32 i;
    s32 years;
    u8(*birthdays)[2];
    s32 today;
    s32 month;
    s32 day;
    s32 until_birthday;
    u8* counter;

    birthdays = g_wldcore_brave_story_birthdays;
    years = days / 365;
    world_script_set_variable(
        EVENT_SCRIPT_VAR_YEAR_COUNT, world_script_get_variable(EVENT_SCRIPT_VAR_YEAR_COUNT) + years);
    days %= 365;
    month = world_script_get_variable(EVENT_SCRIPT_VAR_MONTH);
    day = world_script_get_variable(EVENT_SCRIPT_VAR_DAY);
    today = wldcore_calendar_to_day_of_year(month, day);
    for (i = 0; i < 64; i++) {
        g_main_brave_story_character_ages[i] += years;
        if (i == 0) {
            month = world_script_get_variable(EVENT_SCRIPT_VAR_PLAYER_BIRTH_MONTH);
            day = world_script_get_variable(EVENT_SCRIPT_VAR_PLAYER_BIRTH_DAY);
        } else {
            month = birthdays[i][0];
            day = birthdays[i][1];
        }
        until_birthday = wldcore_calendar_to_day_of_year(month, day) - today;
        if (until_birthday > 0 && days >= until_birthday) {
            g_main_brave_story_character_ages[i] += 1;
        }
        if (g_main_brave_story_character_ages[i] >= 100) {
            g_main_brave_story_character_ages[i] = 99;
        }
    }
    today += days;
    if (today >= 365) {
        world_script_set_variable(
            EVENT_SCRIPT_VAR_YEAR_COUNT, world_script_get_variable(EVENT_SCRIPT_VAR_YEAR_COUNT) + 1);
        today -= 365;
    }
    wldcore_calendar_from_day_of_year(today, &month, &day);
    world_script_set_variable(EVENT_SCRIPT_VAR_MONTH, month);
    world_script_set_variable(EVENT_SCRIPT_VAR_DAY, day);
    for (i = 0; i < g_main_save_proposition_count; i++) {
        if (!(g_main_active_propositions[i].flags & WLDCORE_PROPOSITION_PROGRESS_RETURNED)) {
            if (g_main_active_propositions[i].elapsed_days + days >= g_main_active_propositions[i].assigned_days) {
                g_main_active_propositions[i].elapsed_days = g_main_active_propositions[i].assigned_days;
                g_main_active_propositions[i].flags |= WLDCORE_PROPOSITION_PROGRESS_RETURNED;
            } else if (g_main_active_propositions[i].flags & WLDCORE_PROPOSITION_PROGRESS_RECALLED) {
                g_main_active_propositions[i].elapsed_days++;
                g_main_active_propositions[i].flags |= WLDCORE_PROPOSITION_PROGRESS_RETURNED;
            }
        }
    }
    counter = g_main_proposition_states;
    do {
        if (*counter & WLDCORE_PROPOSITION_STATE_DAYS_MASK) {
            day = (*counter & WLDCORE_PROPOSITION_STATE_DAYS_MASK) - days;
            if (day < 0) {
                day = 0;
            }
            *counter = (*counter & (WLDCORE_PROPOSITION_STATE_FAILURE | WLDCORE_PROPOSITION_STATE_BAD_FAILURE))
                | (day & WLDCORE_PROPOSITION_STATE_DAYS_MASK);
        }
        counter++;
    } while ((s32)counter < (s32)(g_main_proposition_states + 96)); /* Target uses signed SLT. */
}
