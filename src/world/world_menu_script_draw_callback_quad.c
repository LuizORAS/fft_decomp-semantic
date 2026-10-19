#include "fft/world.h"
#include "psx/types.h"

/* Menu-script handler: ask callback instruction->callback_sprite.callback_index
 * for the icon of a row (the literal instruction->callback_sprite.row, or the row
 * being drawn while a list scrolls) and, when it returns one, draw it at the
 * command's x and y with the icon's size, texture origin, CLUT and texture page.
 * Returns the next command. */
menu_instruction_t* world_menu_script_draw_callback_quad(menu_instruction_t* instruction) {
    battle_menu_icon_source_t* (*handler)(s32);
    battle_menu_icon_source_t* entry;
    s32 index;

    handler = g_world_menu_script_callbacks[instruction->callback_sprite.callback_index].icon;
    if (g_world_menu_use_scroll_position == 0) {
        index = instruction->callback_sprite.row;
    } else {
        index = g_world_menu_scroll_offset + g_world_menu_scroll_row_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            index -= 1;
        }
    }
    entry = handler(index);
    if (entry != 0) {
        g_world_menu_script_quad_command.sprite.x = instruction->callback_sprite.x;
        g_world_menu_script_quad_command.sprite.y = instruction->callback_sprite.y;
        /* The icon source stores 16-bit values; the menu script copies their low bytes. */
        g_world_menu_script_quad_command.sprite.w = entry->w;
        g_world_menu_script_quad_command.sprite.h = entry->h;
        g_world_menu_script_quad_command.sprite.u = entry->u;
        g_world_menu_script_quad_command.sprite.v = entry->v;
        g_world_menu_clut = entry->clut;
        g_world_menu_texture_page = entry->tpage;
        world_menu_script_draw_textured_quad((menu_instruction_t*)&g_world_menu_script_quad_command);
    }
    return menu_next_instruction(instruction);
}
