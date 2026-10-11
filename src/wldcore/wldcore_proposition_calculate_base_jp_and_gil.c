#include "fft/wldcore.h"
#include "psx/types.h"

/* JP (row 0) and gil (row 2) of each participant: the proposition's JP and gil
 * amounts, on a failure JP / 10 and gil / 5, on a bad failure both / 20; each
 * then gains the participant's score (row 1). Nothing is done when
 * wldcore_proposition_determine_success set the gate. */
void wldcore_proposition_calculate_base_jp_and_gil(void) {
    const u16* jp_rewards;
    const u16* gil_rewards;
    s32 count;
    s32 i;
    s32 value;

    if (g_wldcore_job_selection.gate == 0) {
        jp_rewards = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JP_AMOUNTS);
        gil_rewards = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_GIL_AMOUNTS);
        count = g_main_active_propositions[g_wldcore_job_selection.proposition_index].participant_count;
        value = 0;

        for (i = 0; i < count; i++) {
            value = jp_rewards[g_wldcore_selected_proposition_row[0].fields.jp_reward - 1];
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
                value = value / 10;
            }
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
                value = value / 20;
            }
            g_wldcore_job_selection.rows[0][i] = value;
            g_wldcore_job_selection.rows[0][i] += g_wldcore_job_selection.rows[1][i];
        }

        for (i = 0; i < count; i++) {
            value = gil_rewards[g_wldcore_selected_proposition_row[0].fields.gil_reward - 1];
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_FAILURE) {
                value = value / 5;
            }
            if (g_wldcore_job_selection.result == WLDCORE_PROPOSITION_RESULT_BAD_FAILURE) {
                value = value / 20;
            }
            g_wldcore_job_selection.rows[2][i] = value;
            g_wldcore_job_selection.rows[2][i] += g_wldcore_job_selection.rows[1][i];
        }
    }
}
