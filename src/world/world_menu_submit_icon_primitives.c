#include "fft/gfx.h"
#include "fft/world.h"

/* Submit the six primitives that compose one menu-frame buffer. */
void world_menu_submit_icon_primitives(menu_icon_sprites_t* prims) {
    gfx_draw_or_append_gpu_primitive(&prims->sprites[3]);
    gfx_draw_or_append_gpu_primitive(&prims->sprites[1]);
    gfx_draw_or_append_gpu_primitive(&prims->sprites[2]);
    gfx_draw_or_append_gpu_primitive(&prims->draw_mode);
    gfx_draw_or_append_gpu_primitive(&prims->sprites[0]);
    gfx_draw_or_append_gpu_primitive(&prims->first_draw_mode);
}
