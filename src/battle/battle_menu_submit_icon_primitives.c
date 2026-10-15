#include "fft/battle.h"

/* Submit the six primitives that compose one menu-frame buffer. */
void battle_menu_submit_icon_primitives(world_menu_icon_record_t* frame) {
    battle_gfx_draw_or_append_gpu_primitive(&frame->base.sprites[3]);
    battle_gfx_draw_or_append_gpu_primitive(&frame->base.sprites[1]);
    battle_gfx_draw_or_append_gpu_primitive(&frame->base.sprites[2]);
    battle_gfx_draw_or_append_gpu_primitive(&frame->base.draw_mode);
    battle_gfx_draw_or_append_gpu_primitive(&frame->base.sprites[0]);
    battle_gfx_draw_or_append_gpu_primitive(&frame->base.first_draw_mode);
}
