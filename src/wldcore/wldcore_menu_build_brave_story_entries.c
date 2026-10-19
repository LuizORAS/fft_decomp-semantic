#include "fft/script.h"
#include "fft/wldcore.h"

/* List the Brave Story's sections for its menu: Fact when EVENT_SCRIPT_VAR_FACTS_ENABLED is set and a
 * fact is; Record and Person always; Job when a proposition is in progress or one has been completed;
 * Unexplored Land and Treasure once one is found; Return always. The upper and lower windows sit on the
 * Unexplored Land and Treasure rows and show once all 16 lands or the first 31 treasures are found. */
void wldcore_menu_build_brave_story_entries(void) {
    s32 count;
    s32 i;

    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count = 0;
    if (script_get_variable(EVENT_SCRIPT_VAR_FACTS_ENABLED) != 0) {
        count = 0;
        for (i = 0; i < EVENT_SCRIPT_VAR_FACT_COUNT; i++) {
            if (script_get_variable(i + EVENT_SCRIPT_VAR_FACT_FIRST) != 0) {
                count++;
            }
        }
        if (count != 0) {
            g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entries[0]
                = WLDCORE_BRAVE_STORY_ROW_FACTS;
            g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count = 1;
        }
    }
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
        .list_window.entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count]
        = WLDCORE_BRAVE_STORY_ROW_STORY_EVENTS;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
        .list_window.entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count + 1]
        = WLDCORE_BRAVE_STORY_ROW_PERSONS;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count += 2;

    count = g_main_save_proposition_count;
    for (i = 0; i < EVENT_SCRIPT_VAR_PROPOSITION_COUNT; i++) {
        if (script_get_variable(i + EVENT_SCRIPT_VAR_PROPOSITION_FIRST) & WLDCORE_PROPOSITION_VARIABLE_COMPLETED) {
            count++;
        }
    }
    if (count != 0) {
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
            .list_window.entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count]
            = WLDCORE_BRAVE_STORY_ROW_PROPOSITIONS;
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count++;
    }

    count = 0;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.upper_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_BLANK;
    for (i = 0; i < EVENT_SCRIPT_VAR_UNEXPLORED_LAND_COUNT; i++) {
        if (script_get_variable(i + EVENT_SCRIPT_VAR_UNEXPLORED_LAND_FIRST) != 0) {
            count++;
        }
    }
    if (count != 0) {
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.upper_window]
            .position.y
            = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count * 0x10 + 0xE;
        if (count >= 0x10) {
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                         .list_window.upper_window]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_BLANK;
        }
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
            .list_window.entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count]
            = WLDCORE_BRAVE_STORY_ROW_UNEXPLORED_LANDS;
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count++;
    }

    count = 0;
    g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.lower_window]
        .flags |= WLDCORE_DISPLAY_OBJECT_BLANK;
    for (i = 0; i < EVENT_SCRIPT_VAR_TREASURE_COUNT; i++) {
        if (script_get_variable(i + EVENT_SCRIPT_VAR_TREASURE_FIRST) != 0) {
            count++;
        }
    }
    if (count != 0) {
        count = 0;
        g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.lower_window]
            .position.y
            = g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count * 0x10 + 0xE;
        for (i = 0; i < 0x1F; i++) {
            if (script_get_variable(i + EVENT_SCRIPT_VAR_TREASURE_FIRST) != 0) {
                count++;
            }
        }
        if (count >= 0x1F) {
            g_wldcore_window_records[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
                                         .list_window.lower_window]
                .flags &= ~WLDCORE_DISPLAY_OBJECT_BLANK;
        }
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
            .list_window.entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count]
            = WLDCORE_BRAVE_STORY_ROW_TREASURES;
        g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count++;
    }
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth]
        .list_window.entries[g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count]
        = WLDCORE_BRAVE_STORY_ROW_LEAVE;
    g_wldcore_menu_stack_records_next[g_wldcore_menu_stack_depth].list_window.entry_count++;
}
