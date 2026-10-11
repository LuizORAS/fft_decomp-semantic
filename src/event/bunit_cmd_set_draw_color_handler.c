#include "fft/event_bunit.h"
#include "psx/types.h"

/* Select the normal or alternate sprite color. */
menu_instruction_t* bunit_cmd_set_draw_color_handler(menu_instruction_t* data) {
    u8 blue;
    if (g_bunit_menu_use_alternate_palette != 0) {
        g_bunit_gfx_sprite_color.r = data->sprite_color.alternate.r;
        g_bunit_gfx_sprite_color.g = data->sprite_color.alternate.g;
        blue = data->sprite_color.alternate.b;
    } else {
        g_bunit_gfx_sprite_color.r = data->sprite_color.normal.r;
        g_bunit_gfx_sprite_color.g = data->sprite_color.normal.g;
        blue = data->sprite_color.normal.b;
    }
    g_bunit_gfx_sprite_color.b = blue;
    return menu_next_instruction(data);
}
