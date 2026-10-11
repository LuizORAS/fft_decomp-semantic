#include "fft/event_bunit.h"
#include "psx/types.h"

/* Stream command: ask row callback cmd[2] for the icon of a row (literal
 * cmd[3], or the row being drawn while a list row is rendered) and, when it
 * returns one, draw it as a textured quad at (cmd[4], cmd[5]) with the
 * icon's size, texture origin, CLUT and texture page. Returns the next
 * command. */
u8* bunit_cmd_draw_row_callback_quad_handler(u8* cmd) {
    menu_icon_source_t* entry;
    s32 row;
    menu_icon_source_t* (*callback)(s32);

    callback = (menu_icon_source_t * (*)(s32)) g_bunit_menu_row_callbacks[cmd[2]];
    if (g_bunit_menu_scroll_list_active == 0) {
        row = cmd[3];
    } else {
        row = g_bunit_menu_scroll_base_index + g_bunit_menu_cursor_row;
        if (g_bunit_menu_scroll_pixel_offset < 0) {
            row -= 1;
        }
    }

    entry = callback(row);
    if (entry != 0) {
        g_bunit_cmd_row_quad[3] = cmd[4];
        g_bunit_cmd_row_quad[4] = cmd[5];
        g_bunit_cmd_row_quad[5] = entry->w;
        g_bunit_cmd_row_quad[6] = entry->h;
        g_bunit_cmd_row_quad[7] = entry->u;
        g_bunit_cmd_row_quad[8] = entry->v;
        g_bunit_gfx_clut_id = entry->clut;
        g_bunit_gfx_texture_page = entry->tpage;
        bunit_cmd_draw_textured_quad_handler(g_bunit_cmd_row_quad);
    }
    return cmd + cmd[1];
}
