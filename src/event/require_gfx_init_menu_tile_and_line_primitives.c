#include "fft/event_require.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Initialise the frame primitives of a status panel buffer: two
 * semi-transparent 16x90 column tiles at x 0x12 and 0x7c, the two vertical
 * edge lines on either side of each, and the draw modes for the frame, text
 * image and item icon texture pages. */
void require_gfx_init_menu_tile_and_line_primitives(battle_menu_status_panel_buffer_t* menu) {
    s32 i;

    SetTile(&menu->tiles[0]);
    SetTile(&menu->tiles[1]);
    SetSemiTrans(&menu->tiles[0], 1);
    SetSemiTrans(&menu->tiles[1], 1);
    setRGB0(&menu->tiles[0], 0x30, 0x30, 0x30);
    setRGB0(&menu->tiles[1], 0x30, 0x30, 0x30);
    menu->tiles[0].x0 = 0x12;
    menu->tiles[0].y0 = 1;
    menu->tiles[0].w = 0x10;
    menu->tiles[0].h = 0x5a;
    menu->tiles[1].x0 = 0x7c;
    menu->tiles[1].y0 = 1;
    menu->tiles[1].w = 0x10;
    menu->tiles[1].h = 0x5a;
    for (i = 0; i < 8; i++) {
        SetLineF2(&menu->lines[i]);
        SetSemiTrans(&menu->lines[i], 1);
    }
    setRGB0(&menu->lines[0], 0x10, 0x10, 0x10);
    setRGB0(&menu->lines[1], 0x20, 0x20, 0x20);
    setRGB0(&menu->lines[2], 0x20, 0x20, 0x20);
    setRGB0(&menu->lines[3], 0x10, 0x10, 0x10);
    setRGB0(&menu->lines[4], 0x10, 0x10, 0x10);
    setRGB0(&menu->lines[5], 0x20, 0x20, 0x20);
    setRGB0(&menu->lines[6], 0x20, 0x20, 0x20);
    setRGB0(&menu->lines[7], 0x10, 0x10, 0x10);
    menu->lines[0].x0 = 0x10;
    menu->lines[0].y0 = 1;
    menu->lines[0].x1 = 0x10;
    menu->lines[0].y1 = 0x5a;
    menu->lines[1].x0 = 0x11;
    menu->lines[1].y0 = 1;
    menu->lines[1].x1 = 0x11;
    menu->lines[1].y1 = 0x5a;
    menu->lines[2].x0 = 0x22;
    menu->lines[2].y0 = 1;
    menu->lines[2].x1 = 0x22;
    menu->lines[2].y1 = 0x5a;
    menu->lines[3].x0 = 0x23;
    menu->lines[3].y0 = 1;
    menu->lines[3].x1 = 0x23;
    menu->lines[3].y1 = 0x5a;
    menu->lines[4].x0 = 0x7a;
    menu->lines[4].y0 = 1;
    menu->lines[4].x1 = 0x7a;
    menu->lines[4].y1 = 0x5a;
    menu->lines[5].x0 = 0x7b;
    menu->lines[5].y0 = 1;
    menu->lines[5].x1 = 0x7b;
    menu->lines[5].y1 = 0x5a;
    menu->lines[6].x0 = 0x8c;
    menu->lines[6].y0 = 1;
    menu->lines[6].x1 = 0x8c;
    menu->lines[6].y1 = 0x5a;
    menu->lines[7].x0 = 0x8d;
    menu->lines[7].y0 = 1;
    menu->lines[7].x1 = 0x8d;
    menu->lines[7].y1 = 0x5a;
    battle_gfx_set_draw_mode_for_texture_page(&menu->draw_modes[0], MENU_TEXTURE_PAGE_FRAME);
    battle_gfx_set_draw_mode_for_texture_page(&menu->draw_modes[1], MENU_TEXTURE_PAGE_TEXT_IMAGES);
    battle_gfx_set_draw_mode_for_texture_page(&menu->draw_modes[2], MENU_TEXTURE_PAGE_ITEM_ICONS);
}
