#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Draws a full-screen semi-transparent tile of grey level |intensity|:
 * positive values subtract (abr 2), negative values add (abr 1). */
void world_gfx_submit_screen_fade_overlay(battle_gfx_fade_overlay_t* prims, s32 intensity) {
    s32 abr;
    s32 level = intensity;

    if (level != 0) {
        SetTile(&prims->tile);
        SetSemiTrans(&prims->tile, 1);
        abr = 2;
        prims->tile.w = 0x100;
        prims->tile.h = 0xF0;
        setRGB0(&prims->tile, 0, 0, 0);
        prims->tile.x0 = 0x80;
        prims->tile.y0 = 0;
        if (level < 0) {
            abr = 1;
            level = -level;
        }
        SetDrawMode(&prims->mode, 0, 0, GetTPage(0, abr, 0x3C0, 0x100), &g_world_gfx_texture_window);
        setRGB0(&prims->tile, level, level, level);
        if (g_world_gfx_draw_primitives_immediately != 0) {
            world_gfx_draw_or_append_gpu_primitive(&prims->mode);
            world_gfx_draw_or_append_gpu_primitive(&prims->tile);
        } else {
            world_gfx_draw_or_append_gpu_primitive(&prims->tile);
            world_gfx_draw_or_append_gpu_primitive(&prims->mode);
        }
        g_world_menu_new_button_input = 0;
    }
}
