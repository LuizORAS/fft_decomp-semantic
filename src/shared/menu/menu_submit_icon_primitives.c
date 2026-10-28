#include "fft/gfx.h"
#include "fft/menu.h"

/* Submit the six primitives that compose one menu-frame buffer. WORLD compiles this text too. */
void menu_submit_icon_primitives(world_menu_icon_sprites_t* frame) {
    gfx_draw_or_append_gpu_primitive(&frame->sprites[3]);
    gfx_draw_or_append_gpu_primitive(&frame->sprites[1]);
    gfx_draw_or_append_gpu_primitive(&frame->sprites[2]);
    gfx_draw_or_append_gpu_primitive(&frame->draw_mode);
    gfx_draw_or_append_gpu_primitive(&frame->sprites[0]);
    gfx_draw_or_append_gpu_primitive(&frame->first_draw_mode);
}
