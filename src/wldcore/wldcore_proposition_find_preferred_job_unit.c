/* The target keeps &g_wldcore_proposition_outcome.proposition_index in s5 and
 * reaches reporter_party_index, 12 bytes below, off the same base, which is
 * why it saves one more callee-saved register than a per-field access would.
 *
 * `base` stays an explicit local so the job-list offset is computed once in
 * the preheader and the inner add uses two registers. The slot test is
 * written `slots[0] > slots[1]`; the reversed spelling swaps the slots[] load
 * pair and costs instructions.
 *
 * The empty asm statement is explained where it stands. */
#include "fft/wldcore.h"
#include "psx/types.h"

/* The unit the report singles out, as a roster index + 1 (0 for none): a unit sent whose job
 * category is the row's preferred job, preferring one other than the reporting unit; failing that,
 * one other than the reporting unit whose job is in the row's job list (record 14, nine jobs), the
 * only one or, of two, the earlier in the list.
 *
 * The target keeps &g_wldcore_proposition_outcome.proposition_index in s5 and
 * reaches reporter_party_index, 12 bytes below, off the same base, which is
 * why it saves one more callee-saved register than a per-field access would.
 *
 * `base` stays an explicit local so the job-list offset is computed once in
 * the preheader and the inner add uses two registers. The slot test is
 * written `slots[0] > slots[1]`; the reversed spelling swaps the slots[] load
 * pair and costs instructions.
 *
 * The empty asm statement is explained where it stands. */
s32 wldcore_proposition_find_preferred_job_unit(void) {
    s32 candidates[4];
    s32 slots[4];
    s32 count;
    s32 i;
    s32 k;
    s32 participant_count;
    s32 party_index;
    s32 base;
    const wldcore_proposition_data_t* messages;
    party_data_t* unit;

    participant_count = g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_count;
    messages = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JOB_LISTS);
    count = 0;

    if (g_wldcore_selected_proposition_row[0].fields.preferred_job != 0) {
        for (i = 0; i < participant_count; i++) {
            party_index
                = g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_indices[i];
            unit = wldcore_unit_get_party_data_pointer(party_index);
            if (g_wldcore_selected_proposition_row[0].fields.preferred_job - 1
                == wldcore_proposition_get_job_category(unit->job_id)) {
                candidates[count] = party_index;
                count++;
            }
        }
    }

    if (count != 0) {
        for (i = 0; i < count; i++) {
            if (candidates[i] != g_wldcore_proposition_outcome.reporter_party_index) {
                return candidates[i] + 1;
            }
        }
        return candidates[0] + 1;
    }

    base = (g_wldcore_selected_proposition_row[0].fields.job_list_index - 1) * 9;
    for (i = 0; i < participant_count; i++) {
        party_index
            = g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_indices[i];
        if (party_index != g_wldcore_proposition_outcome.reporter_party_index) {
            unit = wldcore_unit_get_party_data_pointer(party_index);
            for (k = 0; k < 9; k++) {
                if (messages->message_adjustments[base + k] == unit->job_id) {
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
