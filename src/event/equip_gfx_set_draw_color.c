#include "fft/event_equip.h"
#include "fft/menu.h"

void equip_gfx_set_draw_color(const rgb24_t* data) {
    g_menu_gfx_sprite_color.r = data->r;
    g_menu_gfx_sprite_color.g = data->g;
    g_menu_gfx_sprite_color.b = data->b;
}
