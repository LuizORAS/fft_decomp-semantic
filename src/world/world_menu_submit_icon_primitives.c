#include "fft/world.h"

/* Submit the six primitives that compose one menu-frame buffer. */
void world_menu_submit_icon_primitives(menu_icon_sprites_t* prims) {
    world_gfx_draw_or_append_gpu_primitive(&prims->sprites[3]);
    world_gfx_draw_or_append_gpu_primitive(&prims->sprites[1]);
    world_gfx_draw_or_append_gpu_primitive(&prims->sprites[2]);
    world_gfx_draw_or_append_gpu_primitive(&prims->draw_mode);
    world_gfx_draw_or_append_gpu_primitive(&prims->sprites[0]);
    world_gfx_draw_or_append_gpu_primitive(&prims->first_draw_mode);
}
