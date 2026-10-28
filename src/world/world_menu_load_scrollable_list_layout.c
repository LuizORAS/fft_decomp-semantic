#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Find the next list-layout record in a menu script stream, apply its
 * geometry, clamp the cursor and scroll position to the option count, and
 * redraw the visible text ids when a list font is active. */
void world_menu_load_scrollable_list_layout(menu_instruction_t* script) {
    /* Text image upload: the VRAM rectangle and a -1 terminated list of text ids for
     * world_text_render_id_list_to_image_rows. The list length is inferred from the 0x80-byte
     * frame (at -O1 the frame carries 56 further bytes of spill slots). */
    RECT rect;
    s16 text_ids[20];
    world_menu_list_record_t* record;
    menu_instruction_t* instruction = script;
    s32 i;

    while (instruction->header.opcode != MENU_SCRIPT_OP_SCROLLABLE_LIST) {
        if (instruction->header.opcode == MENU_SCRIPT_OP_WORLD_END) {
            return;
        }
        instruction = menu_next_instruction(instruction);
    }
    record = &instruction->list;
    g_world_menu_list_row_record_count = record->skip_count;
    g_world_menu_row_height = record->row_height;
    g_world_menu_list_visible_rows = record->visible_rows;
    g_world_menu_list_glyph_width = record->width_a;
    g_world_menu_list_text_columns = record->width_b;
    g_world_menu_list_image_width = (g_world_menu_list_glyph_width * g_world_menu_list_text_columns) >> 2;
    if (g_world_menu_cursor_position >= g_world_menu_option_count) {
        g_world_menu_cursor_position = g_world_menu_option_count - 1;
    }
    if (g_world_menu_cursor_position - g_world_menu_scroll_offset >= g_world_menu_list_visible_rows) {
        g_world_menu_scroll_offset = g_world_menu_cursor_position;
    }
    if (g_world_menu_cursor_position < g_world_menu_scroll_offset) {
        g_world_menu_scroll_offset = g_world_menu_cursor_position;
    }
    if (g_world_menu_option_count < g_world_menu_list_visible_rows) {
        g_world_menu_scroll_offset = 0;
    } else if (g_world_menu_option_count - g_world_menu_scroll_offset < g_world_menu_list_visible_rows) {
        g_world_menu_scroll_offset = g_world_menu_option_count - g_world_menu_list_visible_rows;
    }
    if (g_world_menu_cursor_position >= g_world_menu_option_count) {
        g_world_menu_cursor_position = g_world_menu_option_count - 1;
    }
    if (g_world_menu_list_text_table != 0) {
        rect.x = 0x240;
        rect.y = 0x130;
        rect.w = g_world_menu_list_image_width;
        rect.h = g_world_menu_list_visible_rows * 16;
        ClearImage(&rect, 0, 0, 0);
        for (i = 0; i < g_world_menu_list_visible_rows; i++) {
            text_ids[i] = g_world_menu_list_entry_ids[i + g_world_menu_scroll_offset];
        }
        text_ids[i] = -1;
        rect.x = 0x240;
        rect.y = 0x130;
        rect.w = g_world_menu_list_image_width;
        rect.h = g_world_menu_row_height;
        world_text_render_id_list_to_image_rows(g_world_menu_list_text_table, text_ids, &rect, 0);
    }
    g_world_menu_list_scroll_direction = 0;
    g_world_menu_list_layout_pending = 0;
}
