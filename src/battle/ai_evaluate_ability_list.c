#include "fft/battle_ai.h"

/* Evaluate each entry of the acting unit's ability list in turn: 34 entries at
 * most, ended early by SKILLSET_ID_NONE. -1 suspends, and the next call
 * resumes inside the loop. */
s32 ai_evaluate_ability_list(void) {
    ai_data_t* ai;
    ai_ability_entry_t* entry;

    ai = &g_ai_data_base;
    if (g_ai_data_base.resume_pending != 0) {
        goto retry; /* resume the suspended evaluation inside the loop */
    }
    g_ai_data_base.ability_counter = 0;
    while (1) {
        entry = &ai->ability_lists[ai->acting_unit_compact_index][ai->ability_counter];
        if (entry->skillset == SKILLSET_ID_NONE) {
            return 0;
        }
        ai_load_ability_entry(entry);
    retry:
        if (ai_evaluate_ability_outcome() == -1) {
            return -1;
        }
        ai->ability_counter += 1;
        if (ai->ability_counter >= AI_ABILITY_LIST_COUNT) {
            return 0;
        }
    }
}
