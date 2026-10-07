#include "fft/battle.h"
#include "psx/types.h"

/* Command and option use their s16 slots' low bytes; item uses its full halfword. */
typedef struct {
    u8 command;
    u8 _unused_01;
    u8 option;
    u8 _padding_03; /* aligns item */
    u16 item;
} battle_menu_selection_t;

/**
 * Resolve the pending action-menu command, option, and item selection.
 *
 * Remaps menu-specific values, publishes the resolved selection, and returns
 * the command, option, and item packed into a single value.
 */
s32 battle_menu_resolve_selection(void) {
    s32 command;
    s32 option;
    u32 item;
    s32 index;
    s32 i;
    s32 menu;
    const battle_menu_selection_t* selection;

    /* The object is four halfwords; command and option use their slots'
     * low bytes and item uses the full halfword at slot 2. */
    selection = (const battle_menu_selection_t*)g_battle_menu_pending_selection;
    command = selection->command;
    option = selection->option;
    item = selection->item;
    g_battle_menu_selected_command = command;
    index = option;
    if (command != 0x12 && (command & 0xFE) != 0xFE) {
        menu = g_battle_menu_current_id;
        for (i = 0; i < 31; i++) {
            if (menu == g_battle_menu_id_records[i].menu_id) {
                if (command != 0xFF) {
                    if (menu == 0x69) {
                        command = g_dead_unit_action;
                        g_battle_menu_selected_action.item_id = (u8)g_dead_unit_result;
                        option = MENU_SELECTION_NONE;
                    } else if (menu == 0x19 || menu >= 0x64) {
                        command = *(g_battle_menu_id_records[i].map + option + 1);
                        option = MENU_SELECTION_NONE;
                    } else {
                        command = *(g_battle_menu_id_records[i].map + command + 1);
                    }
                } else {
                    command = g_battle_menu_id_records[i].map[0];
                }
                break;
            }
        }
        if (i == 31) {
            command = 7;
        }
        if (i < 16) {
            g_battle_menu_resolved_command = command;
        } else if ((u32)(command - 5) < 2) {
            g_battle_menu_selected_action.targeting_type = command;
        }
        if (command == 1) {
            g_battle_menu_restore_pending = 1;
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
            g_battle_menu_resolved_command = command;
        }
        g_battle_menu_selected_command = command;
        if ((option & 0xFE) != 0xFE) {
            option = g_battle_action_menu_skillsets[index];
            /* Attack/Defend, Equip Change/0x04 and Elemental/Jump take no list entry. */
            if ((u32)(option - SKILLSET_ID_ATTACK) < 2 || (u32)(option - SKILLSET_ID_EQUIP_CHANGE) < 2
                || (u32)(option - SKILLSET_ID_ELEMENTAL) < 2) {
                g_battle_menu_selected_action.ability_id = 0;
                item = MENU_SELECTION_NONE;
            } else if (option == ACTION_MENU_PSEUDO_SKILLSET_ANYTHING && g_battle_menu_action_menu_build_result == 0) {
                item = g_battle_menu_anything_ability_id;
            } else {
                item = g_battle_ai_workspace_ptr->ability_list.ids[item] & 0x1FF;
            }
            g_battle_menu_selected_action.skillset = option;
            g_battle_menu_used_skillset_id = option;
            if (item != MENU_SELECTION_NONE) {
                /* Item, Draw Out and Throw select an item. */
                if (option == SKILLSET_ID_ITEM || (u32)(option - SKILLSET_ID_DRAW_OUT) < 2) {
                    g_battle_menu_selected_action.item_id = item;
                } else {
                    g_battle_menu_selected_action.ability_id = item;
                }
                g_battle_menu_used_item_id = item;
            }
        }
    }
    return command | ((option & 0xFF) << 8) | (item << 16);
}
