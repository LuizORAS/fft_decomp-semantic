/* Builds the ability list for the active skillset menu entry (menu entry 4's
 * selected_index selects the skillset; g_battle_action_menu_row_types gives its list type). Each type
 * fills the 0x52-row work buffer at *g_battle_ai_workspace_ptr through its own loader, then
 * the rows are sized, greyed (not enough MP, or silenced for voice abilities)
 * and the window layout at 0x80153c78 is set up for 6 visible rows.
 * WORLD twin of battle_menu_build_ability_list (0x8013fa6c); the two are masked-identical.
 *
 * Matching notes: the layout block is one struct (its values_x/extras_x are s16, so `-= 4`
 * stays an addiu immediate and the constant is not shared); window_x is a
 * separate local from the unit index, and its updates follow the loader
 * calls in each arm. The ff-terminated copy takes an ignored third argument
 * (0 or the row count), as the target's call sites load $a2. */
#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

void world_menu_build_ability_list(s32 mode) {
    s32 unit_index;
    s32 window_x;
    battle_stats_t* unit;
    s32 i;
    s32 type;
    s32 skillset;
    s32 count;
    s32 found;
    s32 columns;
    s32 row_mode;
    s32 row_flags;

    g_world_ability_menu_layout.ids = g_battle_ai_workspace_ptr->ability_list.ids;
    g_world_ability_menu_layout.values = g_battle_ai_workspace_ptr->ability_list.values;
    g_world_ability_menu_layout.extras = g_battle_ai_workspace_ptr->ability_list.extras;
    unit_index = g_world_unit_view_battle_id;
    unit = world_unit_get_battle_stats_for_stored();
    type = g_battle_action_menu_row_types[g_world_menu_thread_menu_data[4].selected_index];
    skillset = g_world_action_menu_skillsets[g_world_menu_thread_menu_data[4].selected_index];
    g_world_menu_preview_action.skillset = skillset;
    for (i = 0; i < 0x50; i++) {
        g_battle_ai_workspace_ptr->ability_list.ids[i] = 0xFFFF;
        g_battle_ai_workspace_ptr->ability_list.mp_costs[i] = 0xFF;
        g_battle_ai_workspace_ptr->ability_list.bytes_23e[i] = 1;
        g_battle_ai_workspace_ptr->ability_list.values[i] = 0;
        g_battle_ai_workspace_ptr->ability_list.extras[i] = 0xFFFF;
        g_battle_ai_workspace_ptr->ability_list.flags[i] = 0;
        g_battle_ai_workspace_ptr->ability_list.bytes_2e2[i] = 1;
        g_battle_ai_workspace_ptr->ability_list.enabled[i] = 0xFF;
        g_world_menu_ability_display_flags[i] = 0;
    }
    g_battle_ai_workspace_ptr->ability_list.mp_costs[0x50] = 0xFF;
    g_battle_ai_workspace_ptr->ability_list.bytes_23e[0x50] = 0xFF;
    g_battle_ai_workspace_ptr->ability_list.flags[0x50] = 0xFF;
    g_battle_ai_workspace_ptr->ability_list.bytes_2e2[0x50] = 0xFF;
    g_battle_ai_workspace_ptr->ability_list.enabled[0x50] = 0xFF;

    if (type == ACTION_MENU_TYPE_DEFAULT) {
        battle_menu_get_unit_skillset_ability_data(unit_index, skillset,
            (s16*)g_battle_ai_workspace_ptr->ability_list.ids, g_battle_ai_workspace_ptr->ability_list.mp_costs,
            g_battle_ai_workspace_ptr->ability_list.bytes_23e, 0, g_battle_ai_workspace_ptr->ability_list.flags,
            g_battle_ai_workspace_ptr->ability_list.bytes_2e2);
        world_script_copy_bytes(
            g_battle_ai_workspace_ptr->ability_list.enabled, g_battle_ai_workspace_ptr->ability_list.bytes_23e, 0x50);
    }
    if (type == ACTION_MENU_TYPE_ITEM_INVENTORY) {
        battle_menu_display_item_inventory_ability(
            unit_index, skillset, g_battle_ai_workspace_ptr->ability_list.mp_costs);
    }
    if (type == ACTION_MENU_TYPE_WEAPON_INVENTORY) {
        battle_menu_load_throw_abilities(unit_index, skillset, g_battle_ai_workspace_ptr->ability_list.mp_costs);
    }
    if (type == ACTION_MENU_TYPE_ARITHMETICKS) {
        if (mode == 2) {
            battle_menu_load_math_skill_attributes(unit_index, skillset, g_battle_ai_workspace_ptr->ability_list.ids);
        }
        if (mode == 1) {
            battle_menu_load_math_skill_multiples(unit_index, skillset, g_battle_ai_workspace_ptr->ability_list.ids);
        }
        if (mode == 0) {
            battle_menu_collect_calculator_abilities(
                unit_index, skillset, (s16*)g_battle_ai_workspace_ptr->ability_list.ids);
        }
    }
    if (type == ACTION_MENU_TYPE_MONSTER) {
        battle_menu_collect_monster_skill_abilities(unit_index, skillset,
            (s16*)g_battle_ai_workspace_ptr->ability_list.ids, 0, g_battle_ai_workspace_ptr->ability_list.flags);
    }
    if (type == ACTION_MENU_TYPE_KATANA_INVENTORY) {
        battle_menu_load_draw_out_abilities(unit_index, skillset, g_battle_ai_workspace_ptr->ability_list.mp_costs);
    }
    if (type == ACTION_MENU_TYPE_CHARGE) {
        battle_menu_load_charge_skillset(unit_index, skillset, g_battle_ai_workspace_ptr->ability_list.ids,
            g_battle_ai_workspace_ptr->ability_list.mp_costs, g_battle_ai_workspace_ptr->ability_list.bytes_2e2);
        world_script_copy_bytes(
            g_battle_ai_workspace_ptr->ability_list.enabled, g_battle_ai_workspace_ptr->ability_list.mp_costs, 0x50);
    }
    window_x = 0xAC;
    if (type != ACTION_MENU_TYPE_ARITHMETICKS) {
        mode = 0;
    }

    g_world_ability_menu_layout.values_mode = 3;
    g_world_ability_menu_layout.extras_mode = 3;
    g_world_ability_menu_layout.values_x = 0x6A;
    g_world_ability_menu_layout.extras_x = 0x7E;
    g_world_ability_menu_layout.ids_mode = 0;
    if (g_world_menu_row_type_confirm_actions[type][1] == 1) {
        /* The target also passes 0 in a2 to this two-argument copy. */
        count = ((s32 (*)(void*, void*, s32))world_menu_widen_bytes_to_halfwords)(
            g_battle_ai_workspace_ptr->ability_list.ids, g_battle_ai_workspace_ptr->ability_list.mp_costs, 0);
        world_copy_bytes_to_s16_array((s16*)g_battle_ai_workspace_ptr->ability_list.extras,
            g_battle_ai_workspace_ptr->ability_list.bytes_2e2, count);
        found = 0;
        for (i = 0; i < count; i++) {
            g_battle_ai_workspace_ptr->ability_list.values[i]
                = g_main_item_quantities[g_battle_ai_workspace_ptr->ability_list.ids[i]];
            if (g_battle_ai_workspace_ptr->ability_list.values[i] == 0) {
                g_world_menu_ability_display_flags[i] = 4;
            } else {
                found = 1;
            }
        }
        row_mode = 7;
        if (found != 0) {
            window_x -= 4;
        } else {
            g_world_ability_menu_layout.extras_mode = 2;
            world_copy_bytes_to_s16_array((s16*)g_battle_ai_workspace_ptr->ability_list.values,
                g_battle_ai_workspace_ptr->ability_list.bytes_2e2, count);
            window_x = 0x94;
            row_mode = 8;
        }
        row_flags = TEXT_ID_ITEM_NAME_BASE;
        g_world_ability_menu_layout.values_x -= 4;
        g_world_ability_menu_layout.extras_x -= 4;
        columns = 0x11;
        if (type == ACTION_MENU_TYPE_KATANA_INVENTORY) {
            columns = 0x17;
        }
    } else {
        found = 0;
        for (count = 0; count < 0x40; count++) {
            if (g_battle_ai_workspace_ptr->ability_list.ids[count] == 0xFFFF) {
                break;
            }
        }
        if (type == ACTION_MENU_TYPE_DEFAULT) {
            for (i = 0; i < count; i++) {
                if (g_battle_ai_workspace_ptr->ability_list.mp_costs[i] != 0) {
                    found = 1;
                    break;
                }
            }
        }
        if (type == ACTION_MENU_TYPE_ARITHMETICKS && mode > 0) {
            window_x = 0x88;
            g_world_ability_menu_layout.values_mode = 2;
            g_world_ability_menu_layout.extras_mode = 2;
            row_mode = 9;
        } else {
            if (found != 0 && type == ACTION_MENU_TYPE_DEFAULT) {
                /* The target also passes the row count in a2 to this two-argument copy. */
                ((s32 (*)(void*, void*, s32))world_menu_widen_bytes_to_halfwords)(
                    g_battle_ai_workspace_ptr->ability_list.values, g_battle_ai_workspace_ptr->ability_list.mp_costs,
                    count);
                window_x -= 8;
                world_copy_bytes_to_s16_array((s16*)g_battle_ai_workspace_ptr->ability_list.extras,
                    g_battle_ai_workspace_ptr->ability_list.bytes_2e2, count);
                row_mode = 6;
                g_world_ability_menu_layout.values_x -= 8;
                g_world_ability_menu_layout.extras_x -= 8;
            } else {
                world_copy_bytes_to_s16_array((s16*)g_battle_ai_workspace_ptr->ability_list.values,
                    g_battle_ai_workspace_ptr->ability_list.bytes_2e2, count);
                window_x = 0x94;
                row_mode = 5;
                g_world_ability_menu_layout.extras_mode = 2;
                g_world_ability_menu_layout.values_x -= 4;
                g_world_ability_menu_layout.extras_x -= 4;
            }
        }
        row_flags = TEXT_ID_ABILITY_NAME_BASE;
        columns = 5;
    }

    g_world_menu_thread_menu_data[3].inner_width = window_x;
    g_world_menu_thread_menu_data[3].window_width = window_x;
    g_world_menu_thread_menu_data[3].overall_width = window_x;
    g_world_menu_thread_menu_data[3].header_id = row_mode;
    g_world_menu_thread_menu_data[3].select_text_table = columns;
    for (i = 0; i < count; i++) {
        g_battle_ai_workspace_ptr->ability_list.ids[i] |= row_flags;
        if (type == ACTION_MENU_TYPE_DEFAULT) {
            if (unit->mp - g_battle_ai_workspace_ptr->ability_list.mp_costs[i] < 0
                && g_battle_ai_workspace_ptr->ability_list.enabled[i] != 0) {
                g_world_menu_ability_display_flags[i] = 8;
            }
        }
        if (type == ACTION_MENU_TYPE_DEFAULT || type == ACTION_MENU_TYPE_MONSTER) {
            if ((g_battle_ai_workspace_ptr->ability_list.flags[i] & 2)
                && g_battle_ai_workspace_ptr->ability_list.enabled[i] != 0
                && (unit->status_sets.current[1] & BATTLE_STATUS_BYTE_MASK(BATTLE_STATUS_ID_SILENCE))) {
                g_world_menu_ability_display_flags[i] = 8;
            }
        }
    }
    if (count == 0) {
        count = 1;
        g_world_menu_thread_menu_data[3].header_id = 0;
        g_battle_ai_workspace_ptr->ability_list.ids[0] = TEXT_ID_ABILITY_NAME_BASE;
        g_battle_ai_workspace_ptr->ability_list.values[0] = 0;
        g_battle_ai_workspace_ptr->ability_list.extras[0] = 0;
        g_world_menu_ability_display_flags[0] = 4;
    }
    g_world_menu_thread_menu_data[3].window_y = 0x30;
    if (count >= 7) {
        g_world_ability_menu_layout.hidden_rows = count - 6;
        g_world_ability_menu_layout.visible_rows = 6;
    } else {
        g_world_ability_menu_layout.hidden_rows = 0;
        g_world_ability_menu_layout.visible_rows = count;
        g_world_menu_thread_menu_data[3].window_y += (6 - count) * 8;
    }
    /* Menu-thread parameters 2-4 request a redraw, reset the cursor and
     * request a refresh. */
    g_world_thread_contexts[g_world_thread_current_id].function_parameter_2 = 1;
    g_world_thread_contexts[g_world_thread_current_id].function_parameter_3 = 0;
    g_world_thread_contexts[g_world_thread_current_id].function_parameter_4 = 1;
}
