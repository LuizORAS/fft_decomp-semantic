#include "fft/event_bunit.h"
#include "psx/types.h"

/* Set the sprite RGB from the command; g_bunit_menu_use_alternate_palette selects the second
 * (offset +3) colour triple. */
u8* bunit_cmd_set_draw_color_handler(u8* data) {
    u8 blue;
    if (g_bunit_menu_use_alternate_palette != 0) {
        g_bunit_gfx_sprite_color.r = data[5];
        g_bunit_gfx_sprite_color.g = data[6];
        blue = data[7];
    } else {
        g_bunit_gfx_sprite_color.r = data[2];
        g_bunit_gfx_sprite_color.g = data[3];
        blue = data[4];
    }
    g_bunit_gfx_sprite_color.b = blue;
    return data + data[1];
}
