#include "fft/battle.h"
#include "psx/types.h"

/* Submit a menu line box: the twelve lines, then the sprite between its two draw modes. */
void battle_menu_submit_numeric_display_frame_primitives(world_menu_palette_primitives_t* display) {
    s32 index;

    index = 0;
    do {
        battle_gfx_draw_or_append_gpu_primitive(&display->lines[index]);
        index++;
    } while (index < 12);
    battle_gfx_draw_or_append_gpu_primitive(&display->draw_mode);
    battle_gfx_draw_or_append_gpu_primitive(&display->sprite);
    battle_gfx_draw_or_append_gpu_primitive(&display->draw_mode_menu);
}
