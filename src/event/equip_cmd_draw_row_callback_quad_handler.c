#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command: ask row callback cmd->callback_sprite.callback_index for the icon of a row (literal
 * cmd->callback_sprite.row, or the row being drawn while a list row is rendered) and, when it
 * returns one, draw it as a textured quad at (cmd->callback_sprite.x, cmd->callback_sprite.y) with the
 * icon's size, texture origin, CLUT and texture page. Returns the next
 * command. */
menu_instruction_t* equip_cmd_draw_row_callback_quad_handler(menu_instruction_t* cmd) {
    equip_row_callback_t fn;
    world_item_icon_source_t* entry;
    s32 row;
    u16 tpage;
    u16 clut;

    fn = g_equip_menu_list_row_callbacks[cmd->callback_sprite.callback_index];
    if (g_equip_menu_list_row_mode == 0) {
        row = cmd->callback_sprite.row;
    } else {
        row = g_equip_menu_scroll_base_index + g_equip_menu_list_row_index;
        if (g_equip_menu_list_scroll_offset < 0) {
            row--;
        }
    }

    entry = (world_item_icon_source_t*)fn(row);
    if (entry != 0) {
        g_equip_cmd_row_sprite_command.sprite.x = cmd->callback_sprite.x;
        g_equip_cmd_row_sprite_command.sprite.y = cmd->callback_sprite.y;
        g_equip_cmd_row_sprite_command.sprite.w = entry->w;
        g_equip_cmd_row_sprite_command.sprite.h = entry->h;
        g_equip_cmd_row_sprite_command.sprite.u = entry->u;
        g_equip_cmd_row_sprite_command.sprite.v = entry->v;
        clut = entry->clut;
        tpage = entry->tpage;
        g_equip_gfx_clut_id = clut;
        g_equip_gfx_texture_page = tpage;
        equip_cmd_draw_textured_quad_handler((menu_instruction_t*)&g_equip_cmd_row_sprite_command);
    }

    return menu_next_instruction(cmd);
}
