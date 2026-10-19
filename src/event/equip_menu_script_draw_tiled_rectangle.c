#include "fft/event_equip.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Draw a rectangle tiled with 16-pixel textured quads.
 *
 * Kind 2 takes the rectangle from the global tiled rect; otherwise command
 * bytes 5 and 6 pack the full-tile counts and remainders in their nibbles.
 */
menu_instruction_t* equip_menu_script_draw_tiled_rectangle(menu_instruction_t* command) {
    RECT rect;
    s32 base_x;
    s32 base_y;
    s32 full_rows;
    s32 partial_width;
    s32 partial_height;
    s32 otag_index;
    u16 clut;
    s32 full_columns;
    s32 row;
    s32 column;

    otag_index = g_menu_gfx_otag_index;
    clut = g_menu_palette_clut;

    if (command->window.rectangle_source == 2) {
        base_x = g_equip_gfx_tiled_rect.x;
        base_y = g_equip_gfx_tiled_rect.y;
        full_columns = g_equip_gfx_tiled_rect.w / 16;
        partial_width = g_equip_gfx_tiled_rect.w % 16;
        full_rows = g_equip_gfx_tiled_rect.h / 16;
        partial_height = g_equip_gfx_tiled_rect.h % 16;
    } else {
        u8 packed_width;
        u8 packed_height;

        base_x = command->window.x;
        base_y = command->window.y;
        packed_width = command->window.width;
        packed_height = command->window.height;
        full_columns = packed_width >> 4;
        partial_width = packed_width & 15;
        full_rows = packed_height >> 4;
        partial_height = packed_height & 15;
    }

    for (row = 0; row < full_rows; row++) {
        rect.x = base_x;
        rect.y = base_y + row * 16;
        rect.w = partial_width;
        rect.h = 16;
        menu_gfx_enqueue_textured_quad(
            &rect, 8, 8, 0, g_menu_gfx_semitransparency, g_menu_atlas_texture_page, clut, otag_index);
        for (column = 0; column < full_columns; column++) {
            rect.x = base_x + partial_width + column * 16;
            rect.y = base_y + row * 16;
            rect.w = 16;
            rect.h = 16;
            menu_gfx_enqueue_textured_quad(
                &rect, 8, 8, 0, g_menu_gfx_semitransparency, g_menu_atlas_texture_page, clut, otag_index);
        }
    }

    if (partial_height != 0) {
        rect.x = base_x;
        rect.y = base_y + row * 16;
        rect.w = partial_width;
        rect.h = partial_height;
        menu_gfx_enqueue_textured_quad(
            &rect, 8, 8, 0, g_menu_gfx_semitransparency, g_menu_atlas_texture_page, clut, otag_index);
        for (column = 0; column < full_columns; column++) {
            rect.x = base_x + partial_width + column * 16;
            rect.y = base_y + row * 16;
            rect.w = 16;
            rect.h = partial_height;
            menu_gfx_enqueue_textured_quad(
                &rect, 8, 8, 0, g_menu_gfx_semitransparency, g_menu_atlas_texture_page, clut, otag_index);
        }
    }

    command = menu_next_instruction(command);
    return command;
}
