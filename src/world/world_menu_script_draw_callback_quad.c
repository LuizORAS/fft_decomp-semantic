#include "fft/world.h"
#include "psx/types.h"

/* Menu-script handler: ask callback cmd[2] for the icon of a row (literal
 * cmd[3], or the row being drawn while a list scrolls) and, when it returns
 * one, draw it at (cmd[4], cmd[5]) with the icon's size, texture origin,
 * CLUT and texture page. Returns the next command. */
u8* world_menu_script_draw_callback_quad(u8* cmd) {
    s32 (*handler)(s32);
    world_item_icon_source_t* entry;
    s32 index;

    handler = g_world_menu_script_callbacks[cmd[2]];
    if (g_world_menu_use_scroll_position == 0) {
        index = cmd[3];
    } else {
        index = g_world_menu_scroll_offset + g_world_menu_scroll_row_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            index -= 1;
        }
    }
    entry = (world_item_icon_source_t*)handler(index);
    if (entry != 0) {
        g_world_menu_script_quad_command[3] = cmd[4];
        g_world_menu_script_quad_command[4] = cmd[5];
        /* The icon source stores halfwords; the menu script copies their low bytes. */
        g_world_menu_script_quad_command[5] = entry->w;
        g_world_menu_script_quad_command[6] = entry->h;
        g_world_menu_script_quad_command[7] = entry->u;
        g_world_menu_script_quad_command[8] = entry->v;
        g_world_menu_clut = entry->clut;
        g_world_menu_texture_page = entry->tpage;
        world_menu_script_draw_textured_quad(g_world_menu_script_quad_command);
    }
    return cmd + cmd[1];
}
