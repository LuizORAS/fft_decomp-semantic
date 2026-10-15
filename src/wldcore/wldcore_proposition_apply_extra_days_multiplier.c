#include "fft/wldcore.h"
#include "psx/types.h"

/* Scale each unit's JP and gil by the days the units stayed out beyond the row's minimum: the
 * percentage of that day (capped at 9) in records 12 (JP) and 13 (gil). Nothing for a called-back
 * proposition (the gate) or one back before its minimum.
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
    s32 value;

    if (g_wldcore_proposition_outcome.gate != 0) {
        return;
    }
    reward_scale = (const u16*)wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_EXTRA_DAY_JP_SCALES);
    bonus_scale = (const u16*)wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_EXTRA_DAY_GIL_SCALES);
    /* Byte view: the match needs each field offset folded into the symbol
     * address, which a wldcore_proposition_progress_t* base defeats. */
    record = g_wldcore_proposition_outcome.proposition_index * sizeof(wldcore_proposition_progress_t);
    day = ((u8*)g_main_active_propositions)[record + 2] - g_wldcore_selected_proposition_row[0].fields.min_days;
    count = ((u8*)g_main_active_propositions)[record + 5];
    if (day < 0) {
        return;
    }
    if (day >= 10) {
        day = 9;
    }
    for (percent = reward_scale[day], i = 0; i < count; i++) {
        value = g_wldcore_proposition_outcome.rows[0][i];
        value *= percent;
        g_wldcore_proposition_outcome.rows[0][i] = value / 100;
    }
    for (percent = bonus_scale[day], i = 0; i < count; i++) {
        value = g_wldcore_proposition_outcome.rows[2][i];
        value *= percent;
        g_wldcore_proposition_outcome.rows[2][i] = value / 100;
    }
}
