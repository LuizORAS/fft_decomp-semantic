#include "fft/wldcore.h"
#include "psx/types.h"

/* Resolve finished proposition `job_id` before its report: unpack its data
 * row, pick the unit who reports, decide the result and work out the JP and
 * gil, the bonus reward and the extra-days multiplier. */
void wldcore_proposition_prepare_report(s32 job_id) {
    wldcore_unpack_proposition_row(
        g_wldcore_selected_proposition_row, g_main_active_propositions[job_id].proposition_id);
    g_wldcore_job_selection.proposition_index = job_id;
    g_wldcore_job_selection.excluded_party_index = wldcore_proposition_select_report_speaker(job_id);
    wldcore_proposition_determine_success();
    wldcore_proposition_calculate_base_jp_and_gil();
    wldcore_proposition_roll_bonus_reward();
    wldcore_proposition_apply_extra_days_multiplier();
}
