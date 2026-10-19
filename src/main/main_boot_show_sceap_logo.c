#include "fft/main.h"

/* Show the SCEA logo: load SCEAP.DAT (sector 198, a 320x32 image) to the high overlay address on
 * a 320x240 display, fade it in through a subtractive black quad over 32 frames, hold it 180
 * frames, fade it out over 32 and clear the 320x480 VRAM area (before and after). */
void main_boot_show_sceap_logo(void) {
    POLY_F4 polygons[2];
    DR_MODE draw_modes[2];
    u32 otags[2][2];
    /* SetDrawMode uses this as a texture window; zero selects the full window. */
    RECT image_rect = { 0 };
    u32* current_otag;
    s16 intensity;
    int frame;

    main_gfx_set_display_draw(320, 240, 512, 0, 0, 0);
    main_file_load_to_address_checked(DISC_SECTOR_SCEAP_DAT, 0x5000, g_main_heap_high_overlay_load_address);
    frame = 0;

    SetPolyF4(&polygons[0]);
    setRGB0(&polygons[0], 0, 0, 0);
    setXY4(&polygons[0], 0, 0, 320, 0, 0, 240, 320, 240);
    SetSemiTrans(&polygons[0], 1);
    polygons[1] = polygons[0];

    SetDrawMode(&draw_modes[0], 0, 0, 0x40, &image_rect);
    draw_modes[1] = draw_modes[0];

    image_rect.x = 0;
    image_rect.y = 0;
    image_rect.w = 320;
    image_rect.h = 480;
    ClearImage(&image_rect, 0, 0, 0);
    DrawSync(0);
    SetDispMask(1);

    for (; frame < 15; frame++) {
        VSync(0);
    }

    intensity = 248;
    while (intensity >= 0) {
        current_otag = otags[g_main_gfx_screen_polarity];
        ClearOTag(current_otag, 2);
        setRGB0(&g_main_gfx_screen_polarity[polygons], intensity, intensity, intensity);
        AddPrim(current_otag, &polygons[g_main_gfx_screen_polarity]);
        AddPrim(current_otag, &draw_modes[g_main_gfx_screen_polarity]);
        main_boot_draw_sceap_logo_frame(current_otag, (u32*)g_main_heap_high_overlay_load_address);
        intensity -= 8;
    }

    for (frame = 0; frame < 180; frame++) {
        VSync(0);
    }

    intensity = 0;
    while (intensity < 256) {
        if (intensity >= 248) {
            intensity = 255;
        }

        current_otag = otags[g_main_gfx_screen_polarity];
        ClearOTag(current_otag, 2);
        setRGB0(&g_main_gfx_screen_polarity[polygons], intensity, intensity, intensity);
        AddPrim(current_otag, &polygons[g_main_gfx_screen_polarity]);
        AddPrim(current_otag, &draw_modes[g_main_gfx_screen_polarity]);
        main_boot_draw_sceap_logo_frame(current_otag, (u32*)g_main_heap_high_overlay_load_address);
        intensity += 8;
    }

    DrawSync(0);
    image_rect.x = 0;
    image_rect.y = 0;
    image_rect.w = 320;
    image_rect.h = 480;
    ClearImage(&image_rect, 0, 0, 0);
    DrawSync(0);
}
