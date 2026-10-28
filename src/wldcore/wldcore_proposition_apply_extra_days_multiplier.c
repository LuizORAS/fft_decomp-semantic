#include "fft/wldcore.h"
#include "psx/types.h"

/* Scale each unit's JP and gil by the days the units stayed out beyond the row's minimum: the
 * percentage of that day (capped at 9) in records 12 (JP) and 13 (gil). Nothing for a called-back
 * proposition (`recalled`) or one back before its minimum.
 *
 * Byte view of the progress record: see the note at its read. */
void wldcore_proposition_apply_extra_days_multiplier(void) {
    s32 record;
    const u16* reward_scale;
    const u16* bonus_scale;
    s32 day;
    s32 count;
    s32 percent;
    s32 i;

    if (g_wldcore_proposition_outcome.recalled != 0) {
        return;
    }
    reward_scale = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_EXTRA_DAY_JP_SCALES);
    bonus_scale = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_EXTRA_DAY_GIL_SCALES);
    record = g_wldcore_proposition_outcome.proposition_index;
    day = g_main_active_propositions[record].elapsed_days - g_wldcore_selected_proposition_row[0].min_days;
    count = g_main_active_propositions[record].participant_count;
    if (day < 0) {
        return;
    }
    if (day >= 10) {
        day = 9;
    }
    percent = reward_scale[day];
    for (i = 0; i < count; i++) {
        g_wldcore_proposition_outcome.participant_jp[i]
            = g_wldcore_proposition_outcome.participant_jp[i] * percent / 100;
    }
    percent = bonus_scale[day];
    for (i = 0; i < count; i++) {
        g_wldcore_proposition_outcome.participant_gil[i]
            = g_wldcore_proposition_outcome.participant_gil[i] * percent / 100;
    }
}
