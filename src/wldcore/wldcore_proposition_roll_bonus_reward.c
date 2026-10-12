#include "fft/script.h"
#include "fft/wldcore.h"
#include "psx/types.h"

/* Rolls the reward of a successful proposition.
 *
 * Reward kind 1 or 2 (proposition field 10) picks a random script variable
 * still clear in 0x321..0x33f or 0x350..0x35f, falling back to any index;
 * otherwise field 11 == 1 rolls a three-tier random reward (60/30/10%).
 * The chosen pair from proposition data records 10/11 then adds its bonus
 * to every participant's JP award and sets the reward value. The target places
 * the random-reward branch last, hence the goto. */
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
    g_wldcore_proposition_report.reward_type = WLDCORE_PROPOSITION_REWARD_NONE;
    g_wldcore_proposition_report.reward_value = 0;
    if (g_wldcore_proposition_report.result != WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        return;
    }
    discovery_kind = g_wldcore_selected_proposition_row[0].discovery_kind;
    if (discovery_kind == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        g_wldcore_proposition_report.reward_type = WLDCORE_PROPOSITION_REWARD_TREASURE;
        count = 0;
        for (i = 0; i < 31; i++) {
            if (script_get_variable(i + EVENT_SCRIPT_VAR_TREASURE_FIRST) == 0) {
                candidates[count] = i;
                count++;
            }
        }
    } else if (discovery_kind == WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND) {
        count = 0;
        g_wldcore_proposition_report.reward_type = discovery_kind;
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
        g_wldcore_proposition_report.reward_index = candidates[index];
    } else if (g_wldcore_proposition_report.reward_type == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        g_wldcore_proposition_report.reward_index = (rand() * 31) >> 15;
    } else {
        g_wldcore_proposition_report.reward_index = (rand() * 16) >> 15;
    }
    participant_count = g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_count;
    index = (g_wldcore_selected_proposition_row[0].category_index - 1) * 2;
    for (i = 0; i < participant_count; i++) {
        bonus = bonus_table[location_pairs[index + 1] - 1];
        g_wldcore_proposition_report.participant_jp[i] += bonus;
    }
    g_wldcore_proposition_report.reward_value = value_table[location_pairs[index] - 1];
    return;

random_reward:
    if (g_wldcore_selected_proposition_row[0].random_bonus != 1) {
        return;
    }
    g_wldcore_proposition_report.reward_type = WLDCORE_PROPOSITION_REWARD_RANDOM_BONUS;
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
    g_wldcore_proposition_report.reward_index = index;
    participant_count = g_main_active_propositions[g_wldcore_proposition_report.proposition_index].participant_count;
    for (i = 0; i < participant_count; i++) {
        bonus = bonus_table[random_pairs[index].jp_index - 1];
        g_wldcore_proposition_report.participant_jp[i] += bonus;
    }
    g_wldcore_proposition_report.reward_value = value_table[random_pairs[index].gil_index - 1];
}
