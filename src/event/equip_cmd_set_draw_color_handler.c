#include "fft/event_equip.h"
#include "psx/types.h"

/* Stream command: set the sprite RGB; the compact layout uses the second
   colour triple of the command. */
menu_instruction_t* equip_cmd_set_draw_color_handler(menu_instruction_t* cmd) {
    u8 blue;
    if (g_equip_menu_use_alternate_palette != 0) {
        g_equip_gfx_sprite_color.r = cmd->sprite_color.alternate.r;
        g_equip_gfx_sprite_color.g = cmd->sprite_color.alternate.g;
        blue = cmd->sprite_color.alternate.b;
    } else {
        g_equip_gfx_sprite_color.r = cmd->sprite_color.normal.r;
        g_equip_gfx_sprite_color.g = cmd->sprite_color.normal.g;
        blue = cmd->sprite_color.normal.b;
    }
    g_equip_gfx_sprite_color.b = blue;
    return menu_next_instruction(cmd);
}
