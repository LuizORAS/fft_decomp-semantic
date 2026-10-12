#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/**
 * Resolve the pending action-menu command, option, and item selection.
 *
 * Remaps menu-specific values, publishes the resolved selection, and returns
 * the command, option, and item packed into a single value.
 */
s32 world_menu_resolve_selection(void) {
    world_menu_selection_view_t* selection = (world_menu_selection_view_t*)g_world_menu_pending_selection;
    s32 command;
    s32 option;
    u32 item;
    s32 index;
    s32 i;
    s32 menu;

    command = selection->command;
    option = selection->option;
    item = selection->item;
    g_world_menu_selected_command = command;
    index = option;
    if (command != 0x12 && (command & 0xFE) != 0xFE) {
        menu = g_world_menu_current_id;
        for (i = 0; i < 31; i++) {
            if (menu == g_world_menu_command_maps[i].menu_id) {
                if (command != 0xFF) {
                    if (menu == BATTLE_MENU_ID_DEAD_UNIT) {
                        command = g_world_dead_unit_action;
                        g_world_menu_selected_action.item_id = g_world_dead_unit_result;
                        option = MENU_SELECTION_NONE;
                    } else if (menu == 0x19 || menu >= 0x64) {
                        command = *(g_world_menu_command_maps[i].map + option + 1);
                        option = MENU_SELECTION_NONE;
                    } else {
                        command = *(g_world_menu_command_maps[i].map + command + 1);
                    }
                } else {
                    command = g_world_menu_command_maps[i].map[0];
                }
                break;
            }
        }
        if (i == 31) {
            command = 7;
        }
        if (i < 16) {
            g_world_menu_resolved_command = command;
        } else if (command == 5 || command == 6) {
            g_world_menu_selected_action.targeting_type = command;
        }
        if (command == 1) {
            g_world_menu_restore_pending = 1;
        }
        if (command == 12) {
            if (option < 3) {
                s32 option_offset = (option - 1) * 2;
                s32 adjusted_item = item + 12;

                command = option_offset + adjusted_item;
            } else {
                command = option + 13;
            }
            option = MENU_SELECTION_NONE;
            item = MENU_SELECTION_NONE;
            g_world_menu_resolved_command = command;
        }
        g_world_menu_selected_command = command;
        if ((option & 0xFE) != 0xFE) {
            option = g_world_action_menu_skillsets[index];
            /* Attack/Defend, Equip Change/0x04 and Elemental/Jump take no list entry. */
            if (option == SKILLSET_ID_ATTACK || option == SKILLSET_ID_DEFEND || option == SKILLSET_ID_EQUIP_CHANGE
                || option == SKILLSET_ID_BLANK_04 || option == SKILLSET_ID_ELEMENTAL || option == SKILLSET_ID_JUMP) {
                g_world_menu_selected_action.ability_id = 0;
                item = MENU_SELECTION_NONE;
            } else if (option == ACTION_MENU_PSEUDO_SKILLSET_ANYTHING && g_world_menu_monster_skillset_flag == 0) {
                item = g_world_menu_anything_ability_id;
            } else {
                item = g_battle_ai_workspace_ptr->ability_list.ids[item] & 0x1FF;
            }
            g_world_menu_selected_action.skillset = option;
            g_world_menu_resolved_skillset = option;
            if (item != MENU_SELECTION_NONE) {
                if (option == SKILLSET_ID_ITEM || option == SKILLSET_ID_DRAW_OUT || option == SKILLSET_ID_THROW) {
                    g_world_menu_selected_action.item_id = item;
                } else {
                    g_world_menu_selected_action.ability_id = item;
                }
                g_world_menu_resolved_item_or_ability = item;
            }
        }
    }
    return command | ((option & 0xFF) << 8) | (item << 16);
}
