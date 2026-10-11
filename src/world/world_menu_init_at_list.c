#include "fft/battle.h"
#include "fft/world.h"
#include "psx/types.h"

/* Build the unit-name list menu (WORLD twin of option_menu_init_at_list):
 * fetch the unit list on the main stack, write each unit name (0xFE
 * terminated, bracketed by D9 BE / D9 BF when flag 0x40 is set) into the text
 * buffer at g_world_menu_at_list_text, fill the per-row tables, and size `entry` for up to
 * ten visible rows.
 *
 * As in the twin, the slot table is indexed by the loop index and the row
 * tables by the separate accepted-entry count, and the window geometry is
 * written per branch: the store through the layout pointer may alias the
 * entry, so the repeated window_y store is kept.
 */
void world_menu_init_at_list(world_menu_entry_t* entry) {
    s32 count;
    s32 bracketed;
    s32 i;
    u8* text;

    g_world_thread_call_target = (void (*)(void))battle_action_calculate_at_list;
    world_thread_call_on_main_stack(g_world_action_target_list_slots, 0);
    for (i = 0; i < 256; i++) {
        g_world_dead_unit_menu_flags[i] = 0;
        g_world_menu_at_list_flags[i] = 0;
    }
    g_world_text_section_pointers[TEXT_SECTION_AT_LIST] = g_world_menu_at_list_text;
    count = 0;
    bracketed = 0;
    text = g_world_menu_at_list_text;
    for (i = 0; i < 40; i++) {
        battle_stats_t* unit;
        s32 name_index;

        if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_UNIT_ID_MASK) == BATTLE_AT_END_OF_LIST) {
            break;
        }
        g_world_menu_at_list_indices[i] = i + 1;
        unit = battle_unit_get_stats_from_battle_id(g_world_action_target_list_slots[i].unit & BATTLE_AT_UNIT_ID_MASK);
        if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_ACTION_ENTRY) != 0) {
            *text++ = 0xD9;
            *text++ = 0xBE;
        }
        for (name_index = 0; name_index < 16; name_index++) {
            u8 character = unit->name[name_index];
            *text = character;
            if ((character & 0xFE) == 0xFE) {
                break;
            }
            text++;
        }
        if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_ACTION_ENTRY) != 0) {
            *text++ = 0xD9;
            *text++ = 0xBF;
        }
        *text++ = 0xFE;
        if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_ACTION_ENTRY) != 0) {
            g_world_menu_at_list_secondary_values[count] = i - 0x800;
        } else {
            g_world_menu_at_list_primary_values[count] = i - 0x800;
        }
        if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_ACTION_ENTRY) == 0) {
            g_world_menu_at_list_secondary_values[count] = TEXT_ID_ABILITY_NAME_BASE;
        } else {
            bracketed = 1;
            if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_JUMP) != 0) {
                g_world_menu_at_list_primary_values[count] = 0xB012;
            } else {
                s32 value = g_world_action_target_list_slots[i].action_id_low;
                if ((g_world_action_target_list_slots[i].unit & BATTLE_AT_ABILITY_ID_HIGH_BIT) != 0) {
                    value += 0x100;
                }
                g_world_menu_at_list_primary_values[count] = value + TEXT_ID_ABILITY_NAME_BASE;
            }
        }
        if ((unit->initial_team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE) {
            g_world_menu_at_list_flags[count] = 8;
        }
        count++;
    }
    if (count >= 40) {
        count = 39;
    }
    if (bracketed != 0) {
        world_menu_text_layout_t* layout;

        entry->window_y = 0x20;
        layout = entry->context;
        layout->row_count = 10;
        entry->window_x = 0x8C;
        entry->window_y = 0x20;
        entry->window_width = 0xE8;
        entry->text_image.width = 0xE8;
        entry->inner_width = 0xE8;
    } else {
        world_menu_text_layout_t* layout;

        entry->window_y = 0x20;
        layout = entry->context;
        layout->row_count = 10;
        entry->window_x = 0xBE;
        entry->window_y = 0x20;
        entry->window_width = 0x84;
        entry->text_image.width = 0x84;
        entry->inner_width = 0x84;
    }
    if (count < 10) {
        world_menu_text_layout_t* layout;
        world_menu_text_layout_t* hidden_rows_layout;

        entry->window_y = (10 - count) * 8 + 0x20;
        layout = entry->context;
        layout->row_count = count;
        hidden_rows_layout = entry->context;
        hidden_rows_layout->hidden_rows = 0;
    } else {
        world_menu_text_layout_t* layout = entry->context;

        layout->hidden_rows = count - 10;
    }
}
