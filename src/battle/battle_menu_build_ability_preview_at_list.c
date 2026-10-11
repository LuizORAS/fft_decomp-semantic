/*
 * WORLD twin: world_menu_build_ability_preview_at_list
 * (same code, globals and callees rebound).
 */
#include "fft/battle.h"
#include "psx/types.h"

/*
 * Build the AT (turn order) list previewing the ability the player is about to
 * confirm: stage the selected ability or item into the shared action record at
 * g_battle_menu_selected_action, run battle_action_preview_at_list for the selected unit on the
 * main stack to obtain the previewed turn position and the descriptor list,
 * then write the name text and the four halfword columns and size menu entry
 * 52 around them.
 *
 * BATTLE twin of world_menu_build_ability_preview_at_list (WORLD), itself the twin of
 * option_menu_init_at_list from the loop onwards. The constructs below are
 * the WORLD twin's; see
 * src/world/world_menu_build_ability_preview_at_list.c for the evidence behind each one.
 *
 *   - `action` is a pointer local for the shared action record, so the store
 *     lands in the jal delay slot and the argument is `move a1,s0`.
 *   - The turn-value store is written ONCE after the if/else, and `0xb012`
 *     carries no (s16) cast (`ori v0,zero,0xb012`, not `addiu v1,zero,-20462`).
 *   - `text = published` is a PLAIN assignment written as the last statement
 *     before the `if`, so reorg's backward scan pulls it into the `beqz s7`
 *     delay slot.
 */
void battle_menu_build_ability_preview_at_list(void) {
    world_menu_at_list_work_t* work;
    world_menu_entry_t* entry;
    battle_stats_t* stats;
    u8* text;
    battle_action_command_t* action;
    u8* published;
    s32 turn;
    s32 count;
    s32 index;
    s32 bracketed;
    s32 staged_item;
    u16 text_id;
    const u16* at_table;

    at_table = g_battle_ai_workspace_ptr->ability_list.ids;
    work = (world_menu_at_list_work_t*)g_main_heap_companion_overlay_load_address;
    g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_PREVIEW_AT_LIST].select_text_table = 0x13;
    g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_PREVIEW_AT_LIST].context = &g_battle_menu_at_list_layout;
    text_id = at_table[g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_LIST].selected_index];
    g_battle_menu_selected_action.targeting_type = BATTLE_TARGETING_TYPE_TILE;
    entry = &g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_PREVIEW_AT_LIST];
    if ((text_id & 0xf800) == TEXT_ID_ABILITY_NAME_BASE) {
        staged_item = 0;
        g_battle_menu_selected_action.ability_id = text_id - TEXT_ID_ABILITY_NAME_BASE;
    } else {
        staged_item = 1;
        /* Item selection updates only the low byte of the command word. */
        *(u8*)&g_battle_menu_selected_action.item_or_equipment = text_id;
    }
    action = &g_battle_menu_selected_action;
    action->actor_id = g_battle_active_turn_unit.battle_id;
    stats = battle_unit_get_attacker_data_pointer();
    g_battle_thread_call_target = (void (*)(void))battle_action_preview_at_list;
    turn = battle_thread_call_on_main_stack(stats, action, work->entries);
    if (staged_item != 0) {
        work->entries[1].unit |= BATTLE_AT_ACTION_ENTRY;
        work->entries[1].action_id_low = g_battle_menu_selected_action.item_or_equipment;
    }
    count = 0;
    if (turn < 0 || turn == 0xff) {
        g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_PREVIEW_MESSAGE].text_id = 0x1023;
        battle_thread_set_parameters(g_battle_current_thread_id,
            (s32)&g_battle_menu_thread_menu_data[BATTLE_MENU_ID_ABILITY_PREVIEW_MESSAGE], 0, 0);
        count = 0;
        battle_menu_icon_linked_entry_thread();
    }
    for (index = 0; index < 40; index++) {
        work->primary_colors[index] = 0;
        work->secondary_colors[index] = 0;
    }
    published = work->text;
    g_battle_text_section_pointers[TEXT_SECTION_AT_LIST] = published;
    count = 0;
    bracketed = 0;
    /* The target publishes the buffer from a temporary and copies it into the
     * callee-saved register the loop advances. The copy is a plain assignment
     * written as the LAST statement before the `if`: reorg scans back from the
     * branch and pulls it into the delay slot, which is where the target has
     * it. */
    text = published;
    if (turn != 0) {
        turn--;
    }
    for (index = 0; index < 39; index++) {
        battle_at_entry_t* slot = &work->entries[index + 1];
        s32 name_index;

        if ((slot->unit & BATTLE_AT_UNIT_ID_MASK) == BATTLE_AT_END_OF_LIST) {
            break;
        }
        work->entry_numbers[index] = index + 1;
        stats = battle_unit_get_stats_from_battle_id(slot->unit & BATTLE_AT_UNIT_ID_MASK);
        if ((slot->unit & BATTLE_AT_ACTION_ENTRY) != 0) {
            *text++ = 0xd9;
            *text++ = 0xbe;
        }
        for (name_index = 0; name_index < 16; name_index++) {
            u8 character = stats->name[name_index];
            *text = character;
            if ((character & 0xfe) == 0xfe) {
                break;
            }
            text++;
        }
        if ((slot->unit & BATTLE_AT_ACTION_ENTRY) != 0) {
            *text++ = 0xd9;
            *text++ = 0xbf;
        }
        *text++ = 0xfe;
        if ((slot->unit & BATTLE_AT_ACTION_ENTRY) != 0) {
            work->secondary_values[count] = index - 0x800;
        } else {
            work->primary_values[count] = index - 0x800;
        }
        if ((slot->unit & BATTLE_AT_ACTION_ENTRY) == 0) {
            work->secondary_values[count] = TEXT_ID_ABILITY_NAME_BASE;
        } else {
            s32 turn_text_id;
            s32 turn_bias;

            bracketed = 1;
            if ((slot->unit & BATTLE_AT_JUMP) != 0) {
                turn_text_id = 0xb012;
            } else {
                turn_bias = slot->action_id_low;
                if (index == 0 && staged_item != 0) {
                    turn_text_id = turn_bias + 0x3800;
                } else {
                    if ((slot->unit & BATTLE_AT_ABILITY_ID_HIGH_BIT) != 0) {
                        turn_bias += 0x100;
                    }
                    turn_text_id = turn_bias + TEXT_ID_ABILITY_NAME_BASE;
                }
            }
            work->primary_values[count] = turn_text_id;
        }
        if ((stats->initial_team_flags & BATTLE_TEAM_MASK) != BATTLE_TEAM_BLUE) {
            work->secondary_colors[count] = 8;
        }
        count++;
    }
    if (count >= 39) {
        count = 39;
    }
    work->primary_colors[turn] = 4;
    work->secondary_colors[turn] = 4;
    g_battle_menu_at_list_layout.columns.mode[0] = MENU_TEXT_COLUMN_NUMBER_GLYPHS;
    g_battle_menu_at_list_layout.columns.mode[1] = MENU_TEXT_COLUMN_TEXT;
    g_battle_menu_at_list_layout.columns.mode[2] = MENU_TEXT_COLUMN_TEXT;
    if (bracketed != 0) {
        world_menu_scroll_text_layout_t* layout = entry->context;

        layout->columns.row_count = 6;
        entry->window.x = 0x8c;
        entry->window.y = 0x30;
        entry->window.w = 0xe8;
        entry->text_image.width = 0xe8;
        entry->inner_width = 0xe8;
    } else {
        world_menu_scroll_text_layout_t* layout = entry->context;

        layout->columns.row_count = 6;
        entry->window.x = 0xbe;
        entry->window.y = 0x30;
        entry->window.w = 0x84;
        entry->text_image.width = 0x84;
        entry->inner_width = 0x84;
        g_battle_menu_at_list_layout.columns.mode[2] = MENU_TEXT_COLUMN_HIDDEN;
    }
    if (count < 6) {
        world_menu_scroll_text_layout_t* layout;
        world_menu_scroll_text_layout_t* hidden_rows_layout;

        entry->window.y = (6 - count) * 8 + 0x20;
        layout = entry->context;
        layout->columns.row_count = count;
        hidden_rows_layout = entry->context;
        hidden_rows_layout->columns.hidden_rows = 0;
    } else {
        world_menu_scroll_text_layout_t* layout = entry->context;

        layout->columns.hidden_rows = count - 6;
    }
    entry->selected_index = turn;
    if (turn < 3) {
        world_menu_scroll_text_layout_t* layout = entry->context;

        layout->row_offset = 0;
    } else if (turn >= count - 6) {
        world_menu_scroll_text_layout_t* layout = entry->context;

        layout->row_offset = count - 6;
    } else {
        world_menu_scroll_text_layout_t* layout = entry->context;

        layout->row_offset = turn - 3;
    }
    g_battle_menu_at_list_layout.columns.text_ids[0] = work->entry_numbers;
    g_battle_menu_at_list_layout.columns.text_ids[2] = work->secondary_values;
    g_battle_menu_at_list_layout.columns.text_colors[0] = work->primary_colors;
    g_battle_menu_at_list_layout.columns.text_ids[1] = work->primary_values;
    g_battle_menu_at_list_layout.columns.text_colors[1] = work->secondary_colors;
    g_battle_menu_at_list_layout.columns.text_colors[2] = work->secondary_colors;
}
