#include "fft/wldcore.h"
#include "psx/types.h"

/* Scores the current proposition's success chance.
 *
 * A recalled proposition (progress flag 0x02) clears the JP and gil awards and
 * reports result 2. Otherwise each participant's score accumulates
 * four proposition data tables (records 3..6) keyed on the proposition's
 * fields, the participant's job category and its bravery, faith and level
 * tiers; the total maps onto result 0 (>= 0x63), 1 (>= 0x3b) or 2. A named
 * preferred job then gives a 61 in 100 roll to promote the result back to 0. The
 * preferred-job scan reads the party record once, with the scoring loop's
 * leftover index, as the target does. The target builds the row walk pointers
 * from the proposition_index address, which needs the arrays and that 32-bit value to be
 * members of one object (g_wldcore_job_selection). */
void wldcore_proposition_determine_success(void) {
    const u8* job_table_a;
    const u8* job_table_b;
    const u8* tier_table_a;
    const u8* tier_table_b;
    party_data_t* unit;
    s32 participant_count;
    s32 total;
    s32 score;
    s32 tier;
    s32 value;
    s32 i;

    participant_count = g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_count;
    job_table_a = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_CATEGORY_JOB_SCORES);
    job_table_b = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JOB_LIST_JOB_SCORES);
    tier_table_a = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_BRAVERY_LEVEL_SCORES);
    total = 0;
    tier_table_b = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_FAITH_SCORES);

    if (g_main_active_propositions[g_wldcore_proposition_report.proposition_index].flags
        & WLDCORE_PROPOSITION_PROGRESS_RECALLED) {
        for (i = 0; i < participant_count; i++) {
            g_wldcore_proposition_report.participant_gil[i] = 0;
            g_wldcore_proposition_report.participant_jp[i] = 0;
        }
        g_wldcore_proposition_report.recalled = 1;
        g_wldcore_proposition_report.result = WLDCORE_PROPOSITION_RESULT_BAD_FAILURE;
        g_wldcore_proposition_report.reward_type = WLDCORE_PROPOSITION_REWARD_NONE;
        return;
    }

    g_wldcore_proposition_report.recalled = 0;
    for (i = 0; i < participant_count; i++) {
        unit = wldcore_get_party_data_pointer(
            g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_indices[i]);

        g_wldcore_proposition_report.participant_scores[i]
            = job_table_a[(g_wldcore_selected_proposition_row[0].category_index - 1) * 22
                + wldcore_map_job_id_to_category_index(unit->job_id)];
        g_wldcore_proposition_report.participant_scores[i]
            += job_table_b[(g_wldcore_selected_proposition_row[0].job_list_index - 1) * 22
                + wldcore_map_job_id_to_category_index(unit->job_id)];

        value = unit->bravery;
        if (value != 0) {
            tier = (value - 1) / 20;
        } else {
            tier = 0;
        }
        g_wldcore_proposition_report.participant_scores[i]
            += tier_table_a[(g_wldcore_selected_proposition_row[0].job_list_index - 1) * 5 + tier];

        value = unit->faith;
        if (value != 0) {
            tier = (value - 1) / 20;
        } else {
            tier = 0;
        }
        g_wldcore_proposition_report.participant_scores[i]
            += tier_table_b[(g_wldcore_selected_proposition_row[0].job_list_index - 1) * 5 + tier];

        value = unit->level;
        if (value != 0) {
            tier = (value - 1) / 10;
        } else {
            tier = 0;
        }
        score = g_wldcore_proposition_report.participant_scores[i]
            + tier_table_a[(g_wldcore_selected_proposition_row[0].job_list_index - 1) * 10 + tier];
        total += score;
        g_wldcore_proposition_report.participant_scores[i] = score;
    }

    g_wldcore_proposition_report.result = WLDCORE_PROPOSITION_RESULT_SUCCESS;
    if (total < 0x63) {
        g_wldcore_proposition_report.result = WLDCORE_PROPOSITION_RESULT_FAILURE;
    }
    if (total < 0x3B) {
        g_wldcore_proposition_report.result = WLDCORE_PROPOSITION_RESULT_BAD_FAILURE;
    }
    if (g_wldcore_proposition_report.result == WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        return;
    }
    if (g_wldcore_selected_proposition_row[0].preferred_job == 0) {
        return;
    }

    unit = wldcore_get_party_data_pointer(
        g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_indices[i]);
    for (i = 0; i < participant_count; i++) {
        if (g_wldcore_selected_proposition_row[0].preferred_job - 1
            == wldcore_map_job_id_to_category_index(unit->job_id)) {
            if (((rand() * 100) >> 15) < 0x3D) {
                g_wldcore_proposition_report.result = WLDCORE_PROPOSITION_RESULT_SUCCESS;
                return;
            }
        }
    }
}
