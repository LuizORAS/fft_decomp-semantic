#include "fft/main.h"
#include "psx/libgpu.h"

/* Present one frame of the Squaresoft logo fade: swap the display buffers after
 * VSync, upload the 180x16 logo at (70, 112) of the new draw buffer and draw the
 * caller's fade list over it. Returns VSync's result. */
int main_boot_draw_squaresoft_logo(u32* otag, u32* image) {
    RECT destination;
    int elapsed;

    g_main_gfx_screen_polarity = g_main_gfx_screen_polarity == 0;
    DrawSync(0);
    elapsed = VSync(0);

    PutDispEnv(&g_main_gfx_display_envs[g_main_gfx_screen_polarity]);
    PutDrawEnv(&g_main_gfx_draw_envs[g_main_gfx_screen_polarity]);

    destination.x = 70;
    destination.y = g_main_gfx_draw_envs[g_main_gfx_screen_polarity].clip.y + 112;
    destination.w = 180;
    destination.h = 16;
    LoadImage(&destination, image);

    DrawOTag(otag);
    FntFlush(-1);
    return elapsed;
}
