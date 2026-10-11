#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stream command: draw a textured quad of size (cmd[5], cmd[6]) at
 * (cmd[3], cmd[4]) with texture origin (cmd[7], cmd[8]), in the
 * current sprite colour or, for MENU_SCRIPT_OP_UNCOLOURED_QUAD, unshaded. While
 * a list row is rendered, y is offset by the row's position and the scroll
 * offset. Returns the next command. */
u8* bunit_cmd_draw_textured_quad_handler(u8* cmd) {
    RECT rect;
    s32 index;
    u8* table;

    if (g_bunit_menu_scroll_list_active == 0) {
        index = cmd[4];
    } else {
        index = g_bunit_menu_list_row_height * g_bunit_menu_cursor_row + cmd[4] - g_bunit_menu_scroll_pixel_offset;
        if (g_bunit_menu_scroll_pixel_offset < 0) {
            index -= g_bunit_menu_list_row_height;
        }
    }

    rect.x = cmd[3];
    rect.y = index;
    rect.w = cmd[5];
    rect.h = cmd[6];

    table = g_bunit_gfx_sprite_color;
    if (cmd[0] == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        table = 0;
    }

    bunit_gfx_enqueue_textured_quad(&rect, cmd[7], cmd[8], table, g_bunit_gfx_semitrans_enabled,
        g_bunit_gfx_texture_page, g_bunit_gfx_clut_id, g_bunit_gfx_otag_index);
    return cmd + cmd[1];
}
