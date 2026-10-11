#include "fft/event_equip.h"
#include "psx/types.h"

rgb24_t* equip_gfx_get_draw_color(void) {
    return &g_equip_gfx_sprite_color;
}
