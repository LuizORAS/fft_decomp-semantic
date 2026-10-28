#include "fft/main.h"

/* Fade the Squaresoft logo (180x16, g_main_boot_squaresoft_logo_image_words) in from black: a
 * full-screen subtractive quad steps from 248 to 0 over 32 frames while each frame uploads the
 * logo again. It stays on screen until main_boot_fade_out_squaresoft_logo. */
void main_boot_fade_in_squaresoft_logo(void) {
    POLY_F4 polygons[2];
    DR_MODE draw_modes[2];
    u32 otags[2][2];
    /* SetDrawMode uses this as a texture window; zero selects the full window. */
    RECT image_rect = { 0 };
    s16 intensity;
    int frame;
    int buffer;

    intensity = 248;
    main_gfx_set_display_draw(320, 240, 512, 0, 0, 0);

    SetPolyF4(&polygons[0]);
    setRGB0(&polygons[0], 0, 0, 0);
    setXY4(&polygons[0], 0, 0, 320, 0, 0, 256, 320, 256);
    SetSemiTrans(&polygons[0], 1);
    polygons[1] = polygons[0];

    SetDrawMode(&draw_modes[0], 0, 0, 0x40, &image_rect);
    draw_modes[1] = draw_modes[0];

    image_rect.x = 0;
    image_rect.y = 0;
    image_rect.w = 256;
    image_rect.h = 480;
    ClearImage(&image_rect, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);

    for (frame = 0; frame < 15; frame++) {
        VSync(0);
    }

    while (intensity >= 0) {
        buffer = g_main_gfx_screen_polarity;
        ClearOTag(otags[buffer], 2);
        /* Reversed indexing preserves the retail compiler's addu operand order. */
        setRGB0(&g_main_gfx_screen_polarity[polygons], intensity, intensity, intensity);
        AddPrim(otags[buffer], &polygons[g_main_gfx_screen_polarity]);
        AddPrim(otags[buffer], &draw_modes[g_main_gfx_screen_polarity]);
        main_boot_draw_squaresoft_logo_frame(otags[buffer], g_main_boot_squaresoft_logo_image_words);
        intensity -= 8;
    }

    DrawSync(0);
    DrawSync(0);
}
