#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command: ask row callback cmd[2] for the icon of a row (literal
 * cmd[3], or the row being drawn while a list row is rendered) and, when it
 * returns one, draw it as a textured quad at (cmd[4], cmd[5]) with the
 * icon's size, texture origin, CLUT and texture page. Returns the next
 * command. */
u8* equip_cmd_draw_row_callback_quad_handler(u8* cmd) {
    equip_row_callback_t fn;
    world_item_icon_source_t* entry;
    s32 row;
    u16 tpage;
    u16 clut;

    fn = g_equip_menu_list_row_callbacks[cmd[2]];
    if (g_equip_menu_list_row_mode == 0) {
        row = cmd[3];
    } else {
        row = g_equip_menu_scroll_base_index + g_equip_menu_list_row_index;
        if (g_equip_menu_list_scroll_offset < 0) {
            row--;
        }
    }

    entry = (world_item_icon_source_t*)fn(row);
    if (entry != 0) {
        g_equip_cmd_row_sprite_body[0] = cmd[4];
        g_equip_cmd_row_sprite_body[1] = cmd[5];
        g_equip_cmd_row_sprite_body[2] = entry->w;
        g_equip_cmd_row_sprite_body[3] = entry->h;
        g_equip_cmd_row_sprite_body[4] = entry->u;
        g_equip_cmd_row_sprite_body[5] = entry->v;
        clut = entry->clut;
        tpage = entry->tpage;
        g_equip_gfx_clut_id = clut;
        g_equip_gfx_texture_page = tpage;
        equip_cmd_draw_textured_quad_handler(g_equip_cmd_row_sprite_body - 3);
    }

    return cmd + cmd[1];
}
