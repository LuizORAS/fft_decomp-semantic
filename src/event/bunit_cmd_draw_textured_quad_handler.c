#include "fft/event_bunit.h"
#include "psx/types.h"

u8* bunit_cmd_draw_textured_quad_handler(u8* cmd) {
    s16 params[4];
    s32 index;
    u8* table;

    if (g_bunit_menu_scroll_list_active == 0) {
        index = cmd[4];
    } else {
        index = (s16)g_bunit_menu_list_row_height * g_bunit_menu_cursor_row + cmd[4] - g_bunit_menu_scroll_pixel_offset;
        if (g_bunit_menu_scroll_pixel_offset < 0) {
            index -= (s16)g_bunit_menu_list_row_height;
        }
    }

    params[0] = cmd[3];
    params[1] = index;
    params[2] = cmd[5];
    params[3] = cmd[6];

    table = g_bunit_gfx_sprite_color;
    if (cmd[0] == MENU_SCRIPT_OP_UNCOLOURED_QUAD) {
        table = 0;
    }

    bunit_gfx_enqueue_textured_quad((const RECT*)params, cmd[7], cmd[8], table, g_bunit_gfx_semitrans_enabled,
        g_bunit_gfx_texture_page, g_bunit_gfx_clut_id, g_bunit_gfx_otag_index);
    return cmd + cmd[1];
}
