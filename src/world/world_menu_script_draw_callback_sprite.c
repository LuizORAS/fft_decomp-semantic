#include "fft/world.h"
#include "psx/types.h"

/* Ask the selected callback for the literal row's icon, or the current
 * scrolling row's icon, then draw it at the command's position. */
u8* world_menu_script_draw_callback_sprite(u8* cmd) {
    world_menu_callback_sprite_command_t* command = (world_menu_callback_sprite_command_t*)cmd;
    s32 (*handler)(s32);
    world_item_icon_source_t* entry;
    s32 index;

    handler = g_world_menu_script_callbacks[command->callback_index];
    if (g_world_menu_use_scroll_position == 0) {
        index = command->row;
    } else {
        index = g_world_menu_scroll_offset + g_world_menu_scroll_row_offset;
        if (g_world_menu_scroll_pixel_offset < 0) {
            index -= 1;
        }
    }
    entry = (world_item_icon_source_t*)handler(index);
    if (entry != 0) {
        g_world_menu_script_sprite_command.sprite.x = command->x;
        g_world_menu_script_sprite_command.sprite.y = command->y;
        /* The icon source stores halfwords; the menu script copies their low bytes. */
        g_world_menu_script_sprite_command.sprite.w = entry->w;
        g_world_menu_script_sprite_command.sprite.h = entry->h;
        g_world_menu_script_sprite_command.sprite.u = entry->u;
        g_world_menu_script_sprite_command.sprite.v = entry->v;
        g_world_menu_clut = entry->clut;
        g_world_menu_texture_page = entry->tpage;
        world_menu_script_draw_sprite((u8*)&g_world_menu_script_sprite_command);
    }
    return (u8*)command + command->length;
}
