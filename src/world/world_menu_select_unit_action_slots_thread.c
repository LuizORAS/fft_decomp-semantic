#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_select_unit_action_slots_thread. */
void world_menu_select_unit_action_slots_thread(void) {
    s32 i = g_world_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index;
    s32 menu = g_battle_action_menu_row_types[i];
    s32 check_unit = g_battle_menu_skillset_disabled_flags[i];
    battle_menu_record_t* row = &g_world_menu_unit_selection_records[g_world_active_turn_unit.battle_id];
    u8* slot;
    u8* pair;
    s32 j;
    s16 text;

    for (i = (g_world_menu_restore_pending == 0) ? 2 : 0; i >= 0; i--) {
        world_menu_check_action_slot_restrictions(menu, i, check_unit);
        if (menu == ACTION_MENU_TYPE_ARITHMETICKS) {
            slot = &row->bytes[i + 11] + i;
            g_world_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index = slot[0];
            g_world_ability_menu_layout.row_offset = slot[1];
        } else {
            pair = &row->bytes[g_world_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index * 2];
            g_world_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index = pair[3];
            g_world_ability_menu_layout.row_offset = pair[4];
            slot = pair + 3;
        }
        for (j = 0; j < 6; j++) {
            if (menu != ACTION_MENU_TYPE_ARITHMETICKS || i == 0) {
                (&g_world_menu_action_slot_row_actions)[j] = 0x1024;
            } else {
                (&g_world_menu_action_slot_row_actions)[j] = -1;
            }
        }
        if (g_battle_action_menu_row_types[g_world_menu_thread_menu_data[BATTLE_MENU_ID_SKILLSET_LIST].selected_index]
            == ACTION_MENU_TYPE_CHANGE_EQUIPMENT) {
            g_world_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index = 0;
            world_menu_run_equip_thread();
        }
        g_world_thread_call_target = (void (*)(void))world_menu_build_ability_list;
        world_thread_call_on_main_stack(i);
        world_script_pulse_tutorial_wait_value(0xFB);
        world_menu_run_scrolling_ability_list_thread();
        slot[0] = g_world_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index;
        slot[1] = g_world_ability_menu_layout.row_offset;
        if (menu != ACTION_MENU_TYPE_ARITHMETICKS) {
            world_thread_exit_current();
        }
        if (g_world_menu_action_slot_selected_option == -1) {
            if (i == 2) {
                world_thread_exit_current();
            }
            i += 2;
        } else {
            /* The workspace cell is void*; this menu reads it as a u16 text-id list. */
            text = g_battle_ai_workspace_ptr->ability_list
                       .ids[g_world_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index]
                - 0x7000;
            if (i == 2) {
                g_world_menu_selected_action.calculator_type = text;
            }
            if (i == 1) {
                g_world_menu_selected_action.calculator_multiplier = text;
            }
            if (i == 0) {
                g_world_menu_selected_action.ability_id = text;
            }
        }
    }
    world_thread_exit_current();
}
