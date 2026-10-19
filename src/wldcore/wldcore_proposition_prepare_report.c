#include "fft/wldcore.h"
#include "psx/types.h"

/* Resolve finished proposition `proposition_index` before its report: unpack its data
 * row, pick the unit who reports, decide the result and work out the JP and
 * gil, the bonus reward and the extra-days multiplier. */
void wldcore_proposition_prepare_report(s32 proposition_index) {
    wldcore_proposition_unpack_row(
        g_wldcore_selected_proposition_row, g_main_active_propositions[proposition_index].proposition_id);
    g_wldcore_proposition_outcome.proposition_index = proposition_index;
    g_wldcore_proposition_outcome.reporter_party_index = wldcore_proposition_select_report_speaker(proposition_index);
    wldcore_proposition_determine_success();
    wldcore_proposition_calculate_base_jp_and_gil();
    wldcore_proposition_roll_bonus_reward();
    wldcore_proposition_apply_extra_days_multiplier();
}
