#include "fft/world.h"
#include "psx/types.h"

/* The menu-screen library's copy of world_menu_draw_text_columns (the same routine compiled twice):
 * the formation, world map and title lists call this one. */
void world_screen_menu_draw_text_columns(world_menu_entry_t* entry, s32* row_offset, void* buffer) {
    world_menu_text_layout_t* layout = (world_menu_text_layout_t*)entry->text_binding;
    s32 row;
    s32 column;
    s32 index;

    for (row = 0; row < layout->row_count; row++) {
        g_world_menu_text_state.pen.stride = entry->inner_width;
        for (column = 0; column < 3; column++) {
            index = row + *row_offset;
            if (layout->mode[column] == MENU_TEXT_COLUMN_HIDDEN) {
                continue;
            }
            if ((layout->text_ids[column][index] & 0x7FF) == 0x7FF) {
                continue;
            }
            world_menu_set_text_origin(layout->x[column], row * 0x10);
            if (layout->mode[column] != MENU_TEXT_COLUMN_HIDDEN) {
                g_world_menu_text_color = layout->text_colors[column][index];
            }
            g_world_menu_text_state.pen.color = 0;
            if (g_world_menu_text_color == 4) {
                g_world_menu_text_state.pen.color = 0x44444444;
            }
            if (layout->mode[column] == MENU_TEXT_COLUMN_TEXT) {
                world_menu_display_text_entry(layout->text_ids[column][index], buffer, &g_world_menu_text_state.pen);
            } else if (layout->mode[column] == MENU_TEXT_COLUMN_NUMBER) {
                world_display_menu_number_entry(
                    layout->text_ids[column][index], 2, buffer, &g_world_menu_text_state.pen);
            } else if (layout->mode[column] == MENU_TEXT_COLUMN_NUMBER_GLYPHS) {
                world_draw_menu_number_glyphs(layout->text_ids[column][index], 2, buffer, &g_world_menu_text_state.pen);
            }
            g_world_menu_text_color = 0;
            g_world_menu_text_state.pen.color = 0;
        }
    }
}
