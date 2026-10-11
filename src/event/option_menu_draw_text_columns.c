#include "fft/event_option.h"
#include "psx/types.h"

/* g_menu_text_state is used as one aggregate (menu_text_state_t): that is
 * what reproduces the target's single s5 base register and the
 * MEM_IN_STRUCT_P reordering of the g_battle_menu_text_palette_offset load across the color
 * store. */

void option_menu_draw_text_columns(menu_entry_t* entry, s32* row_offset, u8* buffer) {
    menu_text_layout_t* layout = entry->context;
    s32 row;
    s32 column;
    s32 index;

    for (row = 0; row < layout->row_count; row++) {
        g_menu_text_state.pen.stride = entry->inner_width;
        for (column = 0; column < 3; column++) {
            index = row + *row_offset;
            if (layout->mode[column] == MENU_TEXT_COLUMN_HIDDEN) {
                continue;
            }
            if ((layout->text_ids[column][index] & 0x7FF) == 0x7FF) {
                continue;
            }
            battle_menu_set_text_origin(layout->x[column], row * 0x10);
            if (layout->mode[column] != MENU_TEXT_COLUMN_HIDDEN) {
                g_battle_menu_text_palette_offset = layout->text_colors[column][index];
            }
            g_menu_text_state.pen.color = 0;
            if (g_battle_menu_text_palette_offset == 4) {
                g_menu_text_state.pen.color = 0x44444444;
            }
            if (layout->mode[column] == MENU_TEXT_COLUMN_TEXT) {
                battle_menu_display_text_entry(layout->text_ids[column][index], buffer, &g_menu_text_state.pen);
            } else if (layout->mode[column] == MENU_TEXT_COLUMN_NUMBER) {
                battle_display_menu_number_entry(layout->text_ids[column][index], 2, buffer, &g_menu_text_state.pen);
            } else if (layout->mode[column] == MENU_TEXT_COLUMN_NUMBER_GLYPHS) {
                battle_draw_menu_number_glyphs(layout->text_ids[column][index], 2, buffer, &g_menu_text_state.pen);
            }
            g_battle_menu_text_palette_offset = 0;
            g_menu_text_state.pen.color = 0;
        }
    }
}
