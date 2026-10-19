#include "fft/wldcore.h"
#include "psx/types.h"

/* Work out each unit's JP (row 0) and gil (row 2) from the row's amounts (records 8 and 9): whole on
 * a success, JP / 10 and gil / 5 on a failure, both / 20 on a bad failure, each plus the unit's score
 * (row 1). Nothing for a called-back proposition (`recalled`). */
void wldcore_proposition_calculate_base_jp_and_gil(void) {
    const u16* jp_rewards;
    const u16* gil_rewards;
    s32 count;
    s32 i;
    s32 value;

    if (g_wldcore_proposition_outcome.recalled == 0) {
        jp_rewards = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JP_AMOUNTS);
        gil_rewards = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_GIL_AMOUNTS);
        count = g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_count;
        value = 0;

        for (i = 0; i < count; i++) {
            value = jp_rewards[g_wldcore_selected_proposition_row[0].jp_reward - 1];
            if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
                value = value / 10;
            }
            if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
                value = value / 20;
            }
            g_wldcore_proposition_outcome.participant_jp[i] = value;
            g_wldcore_proposition_outcome.participant_jp[i] += g_wldcore_proposition_outcome.participant_scores[i];
        }

        for (i = 0; i < count; i++) {
            value = gil_rewards[g_wldcore_selected_proposition_row[0].gil_reward - 1];
            if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
                value = value / 5;
            }
            if (g_wldcore_proposition_outcome.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
                value = value / 20;
            }
            g_wldcore_proposition_outcome.participant_gil[i] = value;
            g_wldcore_proposition_outcome.participant_gil[i] += g_wldcore_proposition_outcome.participant_scores[i];
        }
    }
}
