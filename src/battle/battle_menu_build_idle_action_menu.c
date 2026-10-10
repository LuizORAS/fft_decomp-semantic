#include "fft/battle.h"

enum {
    IDLE_ACTION_MENU_ENTRY_COUNT = 0x34,
};

/* Open an idle action menu (battle_menu_id_e) for the active-turn unit: place the
 * entry windows beside the unit, restore the row the unit last chose and start
 * the menu-building thread on battle_menu_start_entry_thread.
 *
 * Nothing happens while menu input is blocked. The menu is also skipped while the
 * menus are suspended, a help window is open,
 * g_battle_menu_building_thread_started is set (it is cleared here either way) or
 * menu_id is BATTLE_MENU_ID_NONE.
 *
 * g_battle_menu_idle_action_entry_labels holds two window x bytes per entry, each
 * 0x80 less than the screen x; the second is used when the unit's projected x is
 * 0x101 or more, and a zero byte leaves the entry's x unchanged. The single s16
 * handed to battle_unit_project_misc_to_screen receives x: the routine stores x
 * and y, and the y store lands in stack padding. */
void battle_menu_build_idle_action_menu(s32 menu_id) {
    s16 screen_y;
    s32 i;
    s32 column;
    u8* label_table;
    world_menu_entry_t* entry;

    if (g_battle_menu_input_mode != MENU_INPUT_BLOCKED) {
        if ((g_battle_menu_suspend_state == 0) && (g_battle_menu_help_open == 0)
            && (g_battle_menu_building_thread_started == 0) && (menu_id != BATTLE_MENU_ID_NONE)) {
            g_battle_menu_current_id = menu_id;
            g_battle_menu_thread_menu_data = g_battle_menu_idle_action_entries;
            battle_unit_project_misc_to_screen(
                battle_unit_get_misc_id_by_battle_id(g_battle_active_turn_unit.battle_id), &screen_y);
            i = 0;
            entry = g_battle_menu_thread_menu_data;
            label_table = g_battle_menu_idle_action_entry_labels;
            /* Two window x bytes per entry; a projected x of 0x101 or more uses the second. */
            column = screen_y >= 0x101;
            do {
                /* The offset-plus-base integer sum is byte-required (addu operand order). */
                u8* label = (u8*)(column + (u32)label_table);

                column += 2;
                if (*label != 0) {
                    entry->window_x = *label + 0x80;
                }
                i += 1;
                entry += 1;
            } while (i < IDLE_ACTION_MENU_ENTRY_COUNT);
            battle_menu_restore_selection_for_selected_unit(menu_id);
            /* Entry 8's selected_index. A typed member store moves into the following jal
             * delay slot; this raw access preserves the target's store ordering. */
            *(s16*)((u8*)g_battle_menu_thread_menu_data + 0x218) = 0;
            g_battle_menu_status_scroll_offset = 0;
            battle_menu_clear_entry_selections();
            battle_thread_start(NATIVE_THREAD_SLOT_MENU_BUILDING, battle_menu_start_entry_thread);
            battle_thread_set_parameters(NATIVE_THREAD_SLOT_MENU_BUILDING, menu_id, 0, 0);
        }
        g_battle_menu_building_thread_started = 0;
    }
}
