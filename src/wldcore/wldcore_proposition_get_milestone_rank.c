#include "fft/wldcore.h"

/* The discovery milestone a successful proposition has just reached, as a one-based rank, or 0: a
 * treasure found makes the count of the first 31 treasures, an unexplored land the count of the
 * lands, and the rank is that count's place among g_wldcore_treasure_count_milestones (1, 4, 8, 12,
 * 16, 20, 24, 31) or g_wldcore_unexplored_land_count_milestones (1, 3, 6, 8, 10, 12, 14, 16). */
s32 wldcore_proposition_get_milestone_rank(void) {
    s32 count;
    s32 i;
    s32 index;

    if (g_wldcore_proposition_outcome.result != WLDCORE_PROPOSITION_RESULT_SUCCESS) {
        return 0;
    }
    if (g_wldcore_proposition_outcome.reward_type != WLDCORE_PROPOSITION_REWARD_TREASURE
        && g_wldcore_proposition_outcome.reward_type != WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND) {
        return 0;
    }
    if (g_wldcore_proposition_outcome.reward_type == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        count = 0;
        for (i = 0; i < 31; i++) {
            if (world_script_get_variable(i + EVENT_SCRIPT_VAR_TREASURE_FIRST) != 0) {
                count++;
            }
        }
        for (i = 0; i < 8; i++) {
            if (g_wldcore_treasure_count_milestones[i] == count) {
                break;
            }
        }
        if (i == 8) {
            return 0;
        }
        index = i;
    }
    if (g_wldcore_proposition_outcome.reward_type == WLDCORE_PROPOSITION_REWARD_UNEXPLORED_LAND) {
        count = 0;
        for (i = 0; i < EVENT_SCRIPT_VAR_UNEXPLORED_LAND_COUNT; i++) {
            if (world_script_get_variable(i + EVENT_SCRIPT_VAR_UNEXPLORED_LAND_FIRST) != 0) {
                count++;
            }
        }
        for (i = 0; i < 8; i++) {
            if (g_wldcore_unexplored_land_count_milestones[i] == count) {
                break;
            }
        }
        if (i == 8) {
            return 0;
        }
        index = i;
    }
    return index + 1;
}
