#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Roll the extra reward of a successful proposition. A row that finds something
 * (WLDCORE_PROPOSITION_REWARD_TREASURE or _UNEXPLORED_LAND) picks one not yet found at random, among
 * the first 31 treasures or the 16 unexplored lands (any of them once all are found), and its
 * category's pair in record 10 adds JP to every unit and sets the gil value. A row with random_bonus
 * 1 instead draws one of its category's three pairs in record 11, the first 60 times in 100, the
 * second 30 and the third 10.
 *
 * The target places the random-reward branch last, hence the goto. */
void wldcore_proposition_roll_bonus_reward(void) {
    s32 candidates[32];
    u16* bonus_table;
    u16* value_table;
    /* Discovery-row indexing adds two instructions; retain its byte index. */
    u8* location_pairs;
    wldcore_proposition_reward_pair_t* random_pairs;
    s32 count;
    s32 i;
    s32 index;
    s32 participant_count;
    u16 discovery_kind;
    s32 bonus;

    bonus_table = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_JP_AMOUNTS);
    value_table = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_GIL_AMOUNTS);
    location_pairs = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_DISCOVERY_REWARDS);
    random_pairs = wldcore_proposition_get_data_pointer(WLDCORE_PROPOSITION_DATA_RANDOM_REWARDS);
    g_wldcore_proposition_outcome.reward_type = WLDCORE_PROPOSITION_REWARD_NONE;
    g_wldcore_proposition_outcome.reward_value = 0;
    if (g_wldcore_proposition_outcome.result != WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        return;
    }
    discovery_kind = g_wldcore_selected_proposition_row[0].discovery_kind;
    if (discovery_kind == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        g_wldcore_proposition_outcome.reward_type = WLDCORE_PROPOSITION_REWARD_TREASURE;
        count = 0;
        for (i = 0; i < 31; i++) {
            if (script_get_variable(i + EVENT_SCRIPT_VAR_TREASURE_FIRST) == 0) {
                candidates[count] = i;
                count++;
            }
        }
    } else if (discovery_kind == WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND) {
        count = 0;
        g_wldcore_proposition_outcome.reward_type = discovery_kind;
        for (i = 0; i < 16; i++) {
            if (script_get_variable(i + EVENT_SCRIPT_VAR_UNEXPLORED_LAND_FIRST) == 0) {
                candidates[count] = i;
                count++;
            }
        }
    } else {
        goto random_reward;
    }
    if (count != 0) {
        index = (rand() * count) >> 15;
        g_wldcore_proposition_outcome.reward_index = candidates[index];
    } else if (g_wldcore_proposition_outcome.reward_type == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        g_wldcore_proposition_outcome.reward_index = (rand() * 31) >> 15;
    } else {
        g_wldcore_proposition_outcome.reward_index = (rand() * 16) >> 15;
    }
    participant_count = g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_count;
    index = (g_wldcore_selected_proposition_row[0].category_index - 1) * 2;
    for (i = 0; i < participant_count; i++) {
        bonus = bonus_table[location_pairs[index + 1] - 1];
        g_wldcore_proposition_outcome.participant_jp[i] += bonus;
    }
    g_wldcore_proposition_outcome.reward_value = value_table[location_pairs[index] - 1];
    return;

random_reward:
    if (g_wldcore_selected_proposition_row[0].random_bonus != 1) {
        return;
    }
    g_wldcore_proposition_outcome.reward_type = WLDCORE_PROPOSITION_REWARD_RANDOM_BONUS;
    index = (rand() * 100) >> 15;
    if (index < 40) {
        if (index < 10) {
            index = 2;
        } else {
            index = 1;
        }
    } else {
        index = 0;
    }
    index += (g_wldcore_selected_proposition_row[0].category_index - 1) * 3;
    g_wldcore_proposition_outcome.reward_index = index;
    participant_count = g_main_active_propositions[g_wldcore_proposition_outcome.proposition_index].participant_count;
    for (i = 0; i < participant_count; i++) {
        bonus = bonus_table[random_pairs[index].jp_index - 1];
        g_wldcore_proposition_outcome.participant_jp[i] += bonus;
    }
    g_wldcore_proposition_outcome.reward_value = value_table[random_pairs[index].gil_index - 1];
}
