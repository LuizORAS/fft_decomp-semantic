#include "fft/gfx.h"
#include "fft/menu.h"

/* Submit a menu line box: the twelve lines, then the sprite between its two draw modes. WORLD
 * compiles this text too. */
void menu_submit_numeric_display_frame_primitives(world_menu_palette_primitives_t* display) {
    s32 index;

    for (index = 0; index < 12; index++) {
        gfx_draw_or_append_gpu_primitive(&display->lines[index]);
    }
    gfx_draw_or_append_gpu_primitive(&display->draw_mode);
    gfx_draw_or_append_gpu_primitive(&display->sprite);
    gfx_draw_or_append_gpu_primitive(&display->draw_mode_menu);
}
