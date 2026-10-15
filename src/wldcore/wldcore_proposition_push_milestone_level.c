#include "fft/wldcore.h"

/* Announce a discovery milestone (WLDCORE_MENU_LEVEL_DISCOVERY_MILESTONE): the barkeeper's messages
 * from WLDCORE_TEXT_ID_TREASURE_FOUND_FIRST or WLDCORE_TEXT_ID_LAND_FOUND_FIRST, four for the first
 * milestone, one for each later one (its title). */
void wldcore_proposition_push_milestone_level(void) {
    s32 count;

    count = wldcore_proposition_get_milestone_rank();
    if (g_wldcore_proposition_outcome.reward_type == WLDCORE_PROPOSITION_REWARD_TREASURE) {
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].milestone.current
            = WLDCORE_TEXT_ID_TREASURE_FOUND_FIRST;
    } else {
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].milestone.current
            = WLDCORE_TEXT_ID_LAND_FOUND_FIRST;
    }
    if (count == 1) {
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].milestone.remaining = 4;
    } else {
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].milestone.remaining = 1;
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].milestone.current += 2 + count;
    }
    if (world_thread_is_running(WLDCORE_THREAD_SLOT_MESSAGE) == 0) {
        world_thread_start(WLDCORE_THREAD_SLOT_MESSAGE, world_text_message_box_thread);
    }
    wldcore_proposition_show_next_milestone_message(
        &g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].milestone);
    g_wldcore_menu_stack_types[g_wldcore_menu_stack_depth + 1] = WLDCORE_MENU_LEVEL_DISCOVERY_MILESTONE;
    g_wldcore_menu_stack_depth++;
}
