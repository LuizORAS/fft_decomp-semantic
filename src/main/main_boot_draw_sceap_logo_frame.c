#include "fft/main.h"
#include "psx/libgpu.h"

/* Present one SCEA logo frame: flip the display buffers, wait for the GPU and VSync, upload the
 * 320x32 logo strip at line 104 of the new draw buffer and draw otag (the caller's fade quad)
 * over it. Returns VSync(0)'s value. */
int main_boot_draw_sceap_logo_frame(void* otag, void* image) {
    RECT destination;
    int elapsed;

    g_main_gfx_screen_polarity = g_main_gfx_screen_polarity == 0;
    DrawSync(0);
    elapsed = VSync(0);

    PutDispEnv(&g_main_gfx_display_envs[g_main_gfx_screen_polarity]);
    PutDrawEnv(&g_main_gfx_draw_envs[g_main_gfx_screen_polarity]);

    destination.x = 0;
    destination.y = g_main_gfx_draw_envs[g_main_gfx_screen_polarity].clip.y + 104;
    destination.w = 320;
    destination.h = 32;
    LoadImage(&destination, image);

    DrawOTag(otag);
    FntFlush(-1);
    return elapsed;
}
