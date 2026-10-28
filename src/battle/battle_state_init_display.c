#include "fft/battle.h"

/* Reset the GPU and set up both display buffers for a width x height screen with the given
 * background color and GTE projection: the second buffer lies at y 0xf0 unless the height is
 * 0x1e0, the draw area extends 128 pixels left of the screen, dithering is on and the automatic
 * clear off, and g_battle_deployment_clear_rect is cleared to black. Loads the debug font when
 * g_main_font_print_enabled is set. battle_state_run_battle_setup calls it with 256 x 240 and
 * projection 0x200. */
void battle_state_init_display(s32 width, s32 height, s32 projection, u8 red, u8 green, u8 blue) {
    RECT clear_rect;
    s32 second_y;

    clear_rect = g_battle_deployment_clear_rect;
    second_y = -(height != 0x1e0) & 0xf0;

    SetDispMask(0);
    ResetGraph(1);
    SetGeomOffset(width / 2, height / 2);
    SetGeomScreen(projection);

    SetDefDrawEnv(&g_main_gfx_draw_envs[0], -0x80, 0, width + 0x80, height);
    SetDefDispEnv(&g_main_gfx_display_envs[0], 0, second_y, width, height);
    SetDefDrawEnv(&g_main_gfx_draw_envs[1], -0x80, second_y, width + 0x80, height);
    SetDefDispEnv(&g_main_gfx_display_envs[1], 0, 0, width, height);

    g_main_gfx_draw_envs[0].dtd = g_main_gfx_draw_envs[1].dtd = 1;
    g_main_gfx_draw_envs[0].isbg = g_main_gfx_draw_envs[1].isbg = 0;
    setRGB0(&g_main_gfx_draw_envs[0], red, green, blue);
    setRGB0(&g_main_gfx_draw_envs[1], red, green, blue);

    ClearImage(&clear_rect, 0, 0, 0);
    DrawSync(0);
    if (g_main_font_print_enabled != 0) {
        main_gfx_load_efc_fnt();
    }
    PutDispEnv(&g_main_gfx_display_envs[g_main_gfx_screen_polarity]);
    PutDrawEnv(&g_main_gfx_draw_envs[g_main_gfx_screen_polarity]);
}
