/* The target keeps &g_wldcore_job_selection.proposition_index in s5 and
 * reaches excluded_party_index, 12 bytes below, off the same base, which is
 * why it saves one more callee-saved register than a per-field access would.
 *
 * `base` stays an explicit local so the job-list offset is computed once in
 * the preheader and the inner add uses two registers. The slot test is
 * written `slots[0] > slots[1]`; the reversed spelling swaps the slots[] load
 * pair and costs instructions.
 */
#include "fft/wldcore.h"
#include "psx/types.h"

s32 wldcore_proposition_find_preferred_job_unit(void) {
    s32 candidates[4];
    s32 slots[4];
    s32 count;
    s32 i;
    s32 k;
    s32 participant_count;
    s32 party_index;
    s32 base;
    const u8* jobs;
    party_data_t* unit;

    participant_count = g_main_active_propositions[g_wldcore_job_selection.proposition_index].participant_count;
    jobs = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JOB_LISTS);
    count = 0;

    if (g_wldcore_selected_proposition_row[0].preferred_job != 0) {
        for (i = 0; i < participant_count; i++) {
            party_index = g_main_active_propositions[g_wldcore_job_selection.proposition_index].participant_indices[i];
            unit = wldcore_get_party_data_pointer(party_index);
            if (g_wldcore_selected_proposition_row[0].preferred_job - 1
                == wldcore_map_job_id_to_category_index(unit->job_id)) {
                candidates[count] = party_index;
                count++;
            }
        }
    }

    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (candidates[i] != g_wldcore_job_selection.excluded_party_index) {
                return candidates[i] + 1;
            }
        }
        return candidates[0] + 1;
    }

    base = (g_wldcore_selected_proposition_row[0].job_list_index - 1) * 9;
    for (i = 0; i < participant_count; i++) {
        party_index = g_main_active_propositions[g_wldcore_job_selection.proposition_index].participant_indices[i];
        if (party_index != g_wldcore_job_selection.excluded_party_index) {
            unit = wldcore_get_party_data_pointer(party_index);
            for (k = 0; k < 9; k++) {
                if (jobs[base + k] == unit->job_id) {
                    candidates[count] = party_index;
                    slots[count] = k;
                    count++;
                    break;
                }
            }
        }
    }

    if (count == 1) {
        return candidates[0] + 1;
    }
    if (count == 2) {
        if (slots[0] <= slots[1]) {
            return candidates[0] + 1;
        }
        return candidates[1] + 1;
    }
    return 0;
}
