#include "fft/world.h"
#include "psx/types.h"

/* Draws a list page's three text columns into an image. The copy beside the event program (the
 * OPTION.OUT twin): the AT list and the options menu call it. world_screen_menu_draw_text_columns
 * is the same routine in the menu-screen library. */
void world_menu_draw_text_columns(menu_entry_t* entry, s32* row_offset, u8* buffer) {
    menu_text_layout_t* layout = entry->context;
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
