#include "fft/wldcore.h"
#include "psx/types.h"

/* JP and gil of each participant: the proposition's JP and gil
 * amounts, on a failure JP / 10 and gil / 5, on a bad failure both / 20; each
 * then gains the participant's score. No awards are calculated
 * when the proposition was recalled. */
void wldcore_proposition_calculate_base_jp_and_gil(void) {
    const u16* jp_rewards;
    const u16* gil_rewards;
    s32 count;
    s32 i;
    s32 value;

    if (g_wldcore_job_selection.recalled == 0) {
        jp_rewards = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JP_AMOUNTS);
        gil_rewards = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_GIL_AMOUNTS);
        count = g_main_active_propositions[g_wldcore_job_selection.proposition_index].participant_count;
        value = 0;

        for (i = 0; i < count; i++) {
            value = jp_rewards[g_wldcore_selected_proposition_row[0].jp_reward - 1];
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
                value = value / 10;
            }
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
                value = value / 20;
            }
            g_wldcore_job_selection.participant_jp[i] = value;
            g_wldcore_job_selection.participant_jp[i] += g_wldcore_job_selection.participant_scores[i];
        }

        for (i = 0; i < count; i++) {
            value = gil_rewards[g_wldcore_selected_proposition_row[0].gil_reward - 1];
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
                value = value / 5;
            }
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
                value = value / 20;
            }
            g_wldcore_job_selection.participant_gil[i] = value;
            g_wldcore_job_selection.participant_gil[i] += g_wldcore_job_selection.participant_scores[i];
        }
    }
}
