#include "fft/world.h"
#include "psx/types.h"

void world_gfx_init_scaled_draw_area_packets_3(void* record) {
    world_gfx_texture_window_record_t* view = record;

    view->sprite.w = 0;
    view->texture_window.h = 0;
    world_script_copy_bytes(&view->texture_window, &g_world_gfx_draw_area_template, 8);
}
