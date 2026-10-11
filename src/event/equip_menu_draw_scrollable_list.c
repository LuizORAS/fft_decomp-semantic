#include "fft/event_equip.h"

/* Draw the visible rows of a scrollable list.
 *
 * The 0x10 render command supplies the list geometry; 0x19 terminates a
 * stream that has no list.
 */
void equip_menu_draw_scrollable_list(const u8* script) {
    RECT rect;
    s16 row_ids[20];
    s32 i;
    s16 visible_rows;
    s16 entry_count;
    s16 selected;
    const menu_list_record_t* list;

    while (script[0] != MENU_SCRIPT_OP_SCROLLABLE_LIST) {
        if (script[0] == EQUIP_CMD_END) {
            return;
        }
        script += script[1];
    }

    list = (const menu_list_record_t*)script;
    g_equip_menu_list_row_group_count = list->skip_count;
    g_equip_menu_list_row_height = list->row_height;
    visible_rows = list->visible_rows;
    g_equip_menu_list_visible_rows = visible_rows;
    g_equip_menu_list_glyph_width = list->width_a;
    g_equip_menu_list_text_columns = list->width_b;
    g_equip_menu_list_vram_width = (g_equip_menu_list_glyph_width * g_equip_menu_list_text_columns) >> 2;

    entry_count = g_equip_menu_list_entry_count;
    if (g_equip_menu_selected_list_index >= entry_count) {
        g_equip_menu_selected_list_index = entry_count - 1;
    }
    selected = g_equip_menu_selected_list_index;
    if (selected - g_equip_menu_scroll_base_index >= visible_rows) {
        g_equip_menu_scroll_base_index = selected;
    }
    if (selected < g_equip_menu_scroll_base_index) {
        g_equip_menu_scroll_base_index = selected;
    }
    if (entry_count < visible_rows) {
        g_equip_menu_scroll_base_index = 0;
    } else if (entry_count - g_equip_menu_scroll_base_index < visible_rows) {
        g_equip_menu_scroll_base_index = entry_count - visible_rows;
    }

    if (g_equip_menu_selected_list_index >= g_equip_menu_list_entry_count) {
        g_equip_menu_selected_list_index = g_equip_menu_list_entry_count - 1;
    }

    if (g_equip_menu_list_text_table != 0) {
        rect.x = 0x100;
        rect.y = 0x30;
        rect.w = g_equip_menu_list_vram_width;
        rect.h = g_equip_menu_list_visible_rows * 16;
        ClearImage(&rect, 0, 0, 0);
        for (i = 0; i < g_equip_menu_list_visible_rows; i++) {
            row_ids[i] = g_equip_menu_list_entries[i + g_equip_menu_scroll_base_index];
        }
        row_ids[i] = -1;
        rect.x = 0x100;
        rect.y = 0x30;
        rect.w = g_equip_menu_list_vram_width;
        rect.h = g_equip_menu_list_row_height;
        equip_text_render_id_rows_to_vram(g_equip_menu_list_text_table, row_ids, &rect, 0);
    }

    g_equip_menu_list_scroll_direction = 0;
    g_equip_menu_list_redraw_pending = 0;
}
