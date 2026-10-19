#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "fft/sound.h"
#include "fft/thread.h"

/* Thread of an ability list row (ABILITY_CONFIRM): check the chosen ability for the current menu
 * type and confirm the row.
 *
 * A normal skillset checks the MP cost (0x800A, or 0x1008 for an ability with no charge time) and, with
 * the monster skillsets, Silence for a blocked ability (0x8013, or 0x1009); the Item, Throw and Draw
 * Out lists refuse a greyed row (0x100B, 0x100C, 0x100D). The message shows under the current thread with
 * the help blocked, or with navigation messages off only the confirm sound plays. The row is refused
 * (g_battle_menu_result -1) unless the message is the charge-time warning 0x800A or 0x8013 and the player
 * did not cancel it. */
void battle_menu_validate_skill_selection_thread(void) {
    s32 skill;
    world_menu_entry_t* parameter;
    s32 ct;
    s32 entry;
    s32 mp_cost;
    s32 message;
    s32 blocked;
    battle_stats_t* unit;
    world_menu_ability_list_t* skill_tables;

    skill = g_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index;
    parameter = (world_menu_entry_t*)battle_thread_get_current_parameter_1();
    skill_tables = &g_ai_workspace_ptr->ability_list;
    ct = skill_tables->skill_ct[skill];
    entry = g_battle_ability_menu_layout.text_colors[0][skill];
    mp_cost = skill_tables->mp_costs[skill];
    if (ct == 0xFF) {
        ct = 0;
    }
    message = 0;
    blocked = skill_tables->flags[skill] & 2;
    unit = battle_unit_get_active_turn_unit_stats();
    if (g_battle_active_menu == ACTION_MENU_TYPE_DEFAULT && unit->mp - mp_cost < 0) {
        message = 0x800A;
        if (ct == 0) {
            message = 0x1008;
        }
    }
    g_menu_selected_skill_ct = ct;
    if ((g_battle_active_menu == ACTION_MENU_TYPE_MONSTER || g_battle_active_menu == ACTION_MENU_TYPE_DEFAULT)
        && blocked != 0
        && (unit->status_sets.current[BATTLE_STATUS_BYTE_INDEX(BATTLE_STATUS_ID_SILENCE)]
            & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE))) {
        message = 0x8013;
        if (ct == 0) {
            message = 0x1009;
        }
    }
    if (g_battle_active_menu == ACTION_MENU_TYPE_ITEM_INVENTORY) {
        if (entry != 0) {
            message = 0x100B;
        }
    }
    if (g_battle_active_menu == ACTION_MENU_TYPE_WEAPON_INVENTORY && entry != 0) {
        message = 0x100C;
    }
    if (g_battle_active_menu == ACTION_MENU_TYPE_KATANA_INVENTORY) {
        if (entry != 0) {
            message = 0x100D;
        }
    }
    g_battle_menu_result = -4;
    if (message != 0) {
        if (g_main_game_options.fields.navigation_messages == GAME_OPTION_OFF) {
            sound_queue_sfx_confirm_unless_restoring();
        } else {
            g_battle_menu_modal_open = 1;
            g_battle_menu_selected_option = 0;
            g_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE].text_id = message;
            thread_start(g_thread_current_id - 1, battle_menu_icon_linked_entry_thread);
            thread_set_parameters(g_thread_current_id - 1,
                (s32)&g_menu_thread_menu_data[BATTLE_MENU_ID_ACTION_RESTRICTION_MESSAGE], 0, 0);
            sound_queue_sfx_invalid();
            thread_wait_until_inactive(g_thread_current_id - 1);
            g_battle_menu_modal_open = 0;
        }
        if ((message != 0x800A && message != 0x8013) || g_battle_menu_selected_option == -1) {
            g_battle_menu_result = -1;
        }
    } else {
        sound_queue_sfx_confirm_unless_restoring();
    }
    battle_menu_confirm_action_silently(parameter);
    thread_exit_current();
}
