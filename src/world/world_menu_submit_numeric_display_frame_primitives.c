#include "fft/world.h"
#include "psx/types.h"

/* WORLD twin of battle_menu_submit_numeric_display_frame_primitives. */
void world_menu_submit_numeric_display_frame_primitives(world_menu_palette_primitives_t* frame) {
    s32 i;

    for (i = 0; i < 12; i++) {
        world_gfx_draw_or_append_gpu_primitive(&frame->lines[i]);
    }
    world_gfx_draw_or_append_gpu_primitive(&frame->draw_mode);
    world_gfx_draw_or_append_gpu_primitive(&frame->sprite);
    world_gfx_draw_or_append_gpu_primitive(&frame->draw_mode_menu);
}
