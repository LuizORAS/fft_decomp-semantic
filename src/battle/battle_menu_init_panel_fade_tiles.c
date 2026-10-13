#include "fft/battle.h"
#include "fft/battle_menu.h"
#include "fft/memory.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Initialize the center tile and two nine-tile scanline strips. */
void battle_menu_init_panel_fade_tiles(void) {
    menu_gradient_panel_prims_t* frame;
    s32 i;

    frame = &g_battle_menu_panel_fade_frames[0];

    SetDrawMode(&frame->draw_mode, 0, 0, GetTPage(0, 2, 0x3c0, 0x100), &g_battle_menu_disabled_texture_window);

    SetTile(&frame->center);
    setRGB0(&frame->center, 0, 0, 0);
    SetSemiTrans(&frame->center, 1);
    frame->center.x0 = 0x80;
    frame->center.y0 = 0xac;
    frame->center.w = 0x100;
    frame->center.h = 0x32;

    for (i = 0; i < 9; i++) {
        SetTile(&frame->top[i]);
        setRGB0(&frame->top[i], 0, 0, 0);
        SetSemiTrans(&frame->top[i], 1);
        frame->top[i].w = 0x100;
        frame->top[i].h = 1;
        frame->top[i].x0 = 0x80;
        frame->top[i].y0 = i + 0xa3;

        SetTile(&frame->bottom[i]);
        setRGB0(&frame->bottom[i], 0, 0, 0);
        SetSemiTrans(&frame->bottom[i], 1);
        frame->bottom[i].w = 0x100;
        frame->bottom[i].h = 1;
        frame->bottom[i].x0 = 0x80;
        frame->bottom[i].y0 = i + 0xde;
    }

    copy_bytes_nonempty(
        &g_battle_menu_panel_fade_frames[1], &g_battle_menu_panel_fade_frames[0], sizeof(menu_gradient_panel_prims_t));
}
