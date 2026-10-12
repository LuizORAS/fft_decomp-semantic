#include "fft/battle_gfx.h"
#include "fft/battle_menu.h"
#include "psx/types.h"

/* Submit the frame's twelve lines, sprite and draw modes. */
void battle_menu_submit_numeric_display_frame_primitives(menu_palette_primitives_t* display) {
    s32 index;

    for (index = 0; index < 12; index++) {
        battle_gfx_draw_or_append_gpu_primitive(&display->lines[index]);
    }
    battle_gfx_draw_or_append_gpu_primitive(&display->draw_mode);
    battle_gfx_draw_or_append_gpu_primitive(&display->sprite);
    battle_gfx_draw_or_append_gpu_primitive(&display->draw_mode_menu);
}
