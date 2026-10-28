#include "fft/wldcore.h"

/* Count the propositions that have come back (WLDCORE_PROPOSITION_PROGRESS_RETURNED) and wait for
 * their report at `location`, or anywhere for -1. The Bar shows its job reports when this town has
 * one, and the propositions in progress while the count anywhere is short of
 * g_main_save_proposition_count. */
s32 wldcore_proposition_count_returned(s32 location) {
    s32 i;
    s32 count = 0;
    for (i = 0; i < g_main_save_proposition_count; i++) {
        if (g_main_active_propositions[i].flags & WLDCORE_PROPOSITION_PROGRESS_RETURNED) {
            if (location == -1 || g_main_active_propositions[i].location == location)
                count++;
        }
    }
    return count;
}
