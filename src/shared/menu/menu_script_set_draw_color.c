#include "fft/menu.h"
#include "fft/menu_script.h"

/* Set the sprite RGB from the command; g_menu_use_alternate_palette selects the second
 * (offset +3) colour triple. */
menu_instruction_t* menu_script_set_draw_color(menu_instruction_t* data) {
    u8 blue;
    if (g_menu_use_alternate_palette != 0) {
        g_menu_gfx_sprite_color.r = data->sprite_color.alternate.r;
        g_menu_gfx_sprite_color.g = data->sprite_color.alternate.g;
        blue = data->sprite_color.alternate.b;
    } else {
        g_menu_gfx_sprite_color.r = data->sprite_color.normal.r;
        g_menu_gfx_sprite_color.g = data->sprite_color.normal.g;
        blue = data->sprite_color.normal.b;
    }
    g_menu_gfx_sprite_color.b = blue;
    return menu_next_instruction(data);
}
