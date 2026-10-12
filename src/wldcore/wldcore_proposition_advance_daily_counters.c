#include "fft/wldcore.h"

/* The propositions' share of a day (wldcore_calendar_advance_day): every proposition still out gains
 * an elapsed day and comes back (WLDCORE_PROPOSITION_PROGRESS_RETURNED) when it reaches its assigned
 * days or was called back (WLDCORE_PROPOSITION_PROGRESS_RECALLED). Every nonzero day count in
 * g_main_proposition_states (the wait before a failed proposition is offered again) drops by one, from
 * at most 32, keeping its failure bits. */
void wldcore_proposition_advance_daily_counters(void) {
    s32 i;
    u8* counter;

    for (i = 0; i < g_main_save_proposition_count; i++) {
        if (!(g_main_active_propositions[i].flags & WLDCORE_PROPOSITION_PROGRESS_RETURNED)) {
            g_main_active_propositions[i].elapsed_days++;
            if (g_main_active_propositions[i].assigned_days == g_main_active_propositions[i].elapsed_days
                || (g_main_active_propositions[i].flags & WLDCORE_PROPOSITION_PROGRESS_RECALLED)) {
                g_main_active_propositions[i].flags |= WLDCORE_PROPOSITION_PROGRESS_RETURNED;
            }
        }
    }
    counter = g_main_proposition_states;
    do {
        u32 value = *counter;
        s32 days = value & WLDCORE_PROPOSITION_STATE_DAYS_MASK;
        if (days != 0) {
            if (days > 32) {
                days = 32;
            }
            days--;
            *counter = (value & (WLDCORE_PROPOSITION_STATE_FAILURE | WLDCORE_PROPOSITION_STATE_BAD_FAILURE)) | days;
        }
        counter++;
    } while ((s32)counter < (s32)(g_main_proposition_states + 96)); /* Target uses signed SLT. */
}
