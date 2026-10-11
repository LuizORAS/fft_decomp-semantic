#include "fft/event_bunit.h"
#include "psx/types.h"

/* Draw the selected callback's icon for the literal or current list row,
 * using its texture, dimensions and palette. */
menu_instruction_t* bunit_cmd_draw_row_callback_quad_handler(menu_instruction_t* cmd) {
    menu_icon_source_t* entry;
    s32 row;
    menu_icon_source_t* (*callback)(s32);

    callback = (menu_icon_source_t * (*)(s32)) g_bunit_menu_row_callbacks[cmd->callback_sprite.callback_index];
    if (g_bunit_menu_scroll_list_active == 0) {
        row = cmd->callback_sprite.row;
    } else {
        row = g_bunit_menu_scroll_base_index + g_bunit_menu_cursor_row;
        if (g_bunit_menu_scroll_pixel_offset < 0) {
            row -= 1;
        }
    }

    entry = callback(row);
    if (entry != 0) {
        g_bunit_cmd_row_quad.sprite.x = cmd->callback_sprite.x;
        g_bunit_cmd_row_quad.sprite.y = cmd->callback_sprite.y;
        g_bunit_cmd_row_quad.sprite.w = entry->w;
        g_bunit_cmd_row_quad.sprite.h = entry->h;
        g_bunit_cmd_row_quad.sprite.u = entry->u;
        g_bunit_cmd_row_quad.sprite.v = entry->v;
        g_bunit_gfx_clut_id = entry->clut;
        g_bunit_gfx_texture_page = entry->tpage;
        bunit_cmd_draw_textured_quad_handler((menu_instruction_t*)&g_bunit_cmd_row_quad);
    }
    return menu_next_instruction(cmd);
}
