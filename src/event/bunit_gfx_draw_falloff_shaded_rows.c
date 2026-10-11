#include "fft/event_bunit.h"
#include "psx/types.h"

/* Draw rows of gouraud-textured quads whose vertex brightness falls off with
 * distance from the animated focal point. */
void bunit_gfx_draw_falloff_shaded_rows(s32 y) {
    world_formation_background_draw_state_t work;
    s32 row;
    s32 col;
    s32 brightness;
    s16 u;
    s32 i;

    row = 0;
    work.rect.w = 0x40;
    work.rect.clut = 0x40;
    work.rect.h = 0x20;
    work.rect.tpage = 0x20;
    work.rect.v = 0xD8;
    work.rect.y = y;
    for (i = 0; y < 0x101; y += 0x20, i++) {
        for (col = 0; col < 5; col++) {
            brightness = bunit_gfx_calculate_distance_falloff(col << 6, y, 0xFF, 0x50);
            if (brightness > 0xDC) {
                brightness = 0xDC;
            }
            work.shades[i][col] = brightness;
        }
    }

    work.rect.clut = g_bunit_gfx_background_clut;
    work.rect.tpage = g_bunit_menu_icon_texture_page;
    for (; work.rect.y < 0xF0; work.rect.y += 0x20, row++) {
        if (work.rect.y >= -0x20) {
            for (col = 0; col < 4; col++) {
                work.rect.x = g_bunit_gfx_background_tile_layout[row % 2][col * 2];
                u = g_bunit_gfx_background_tile_layout[row % 2][col * 2 + 1];
                work.rect.u = u;
                work.rgb[0] = work.rgb[1] = work.rgb[2] = work.shades[row][col];
                work.rgb[3] = work.rgb[4] = work.rgb[5] = work.shades[row][col + 1];
                work.rgb[6] = work.rgb[7] = work.rgb[8] = work.shades[row + 1][col];
                work.rgb[9] = work.rgb[10] = work.rgb[11] = work.shades[row + 1][col + 1];
                bunit_gfx_enqueue_gouraud_textured_quad(&work.rect, work.rgb, 0, u == 0x58);
            }
        }
    }
}
