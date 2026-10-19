#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "fft/menu_script.h"
#include "psx/types.h"

/* Stream command: ask row callback cmd->callback_sprite.callback_index for the icon
 * of a row (the literal cmd->callback_sprite.row, or the row being drawn while a
 * list row is rendered) and, when it returns one, draw it as a textured quad at
 * the command's x and y with the icon's size, texture origin, CLUT and texture
 * page. Returns the next command. */
menu_instruction_t* bunit_menu_script_draw_callback_quad(menu_instruction_t* cmd) {
    battle_menu_icon_source_t* entry;
    s32 row;
    battle_menu_icon_source_t* (*callback)(s32);

    callback = g_menu_row_callbacks[cmd->callback_sprite.callback_index].icon;
    if (g_menu_scroll_list_active == 0) {
        row = cmd->callback_sprite.row;
    } else {
        row = g_menu_scroll_base_index + g_menu_list_row_index;
        if (g_menu_list_scroll_offset < 0) {
            row -= 1;
        }
    }

    entry = callback(row);
    if (entry != 0) {
        g_bunit_menu_script_quad_command.sprite.x = cmd->callback_sprite.x;
        g_bunit_menu_script_quad_command.sprite.y = cmd->callback_sprite.y;
        g_bunit_menu_script_quad_command.sprite.w = entry->w;
        g_bunit_menu_script_quad_command.sprite.h = entry->h;
        g_bunit_menu_script_quad_command.sprite.u = entry->u;
        g_bunit_menu_script_quad_command.sprite.v = entry->v;
        g_menu_gfx_clut_id = entry->clut;
        g_menu_gfx_texture_page = entry->tpage;
        bunit_menu_script_draw_textured_quad((menu_instruction_t*)&g_bunit_menu_script_quad_command);
    }
    return menu_next_instruction(cmd);
}
