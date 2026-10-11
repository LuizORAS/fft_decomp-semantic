/* LIBGPU 80022fd0-80023288. */
#include "psx/libgpu.h"

int FntOpen(int x, int y, int width, int height, int background, int characters) {
    RECT window;
    SPRT_8* sprite;
    int i;
    if (g_psyq_gpu_font_window_count >= 8)
        return -1;
    if (g_psyq_gpu_font_window_count == 0)
        g_psyq_gpu_font_allocated_characters = 0;
    g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].wrap_disabled = width == 0;
    if (characters + g_psyq_gpu_font_allocated_characters > 1024)
        characters = 1024 - g_psyq_gpu_font_allocated_characters;
    setRECT(&window, 0, 0, 256, 256);
    SetDrawMode(&g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].mode, 0, 0, g_psyq_gpu_font_tpage, &window);
    /* Retail reaches the TILE through a pointer to the window, the other members through the array symbol. */
    if (background) {
        SetTile(&g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].tile);
        setRGB0(&(g_psyq_gpu_font_windows + g_psyq_gpu_font_window_count)->tile, 0, 0, 0);
        SetSemiTrans(&g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].tile, background == 2);
    }
    setXY0(&(g_psyq_gpu_font_windows + g_psyq_gpu_font_window_count)->tile, x, y);
    setWH(&(g_psyq_gpu_font_windows + g_psyq_gpu_font_window_count)->tile, width, height);
    g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].character_limit = characters;
    g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].text_count = 0;
    g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].text
        = g_psyq_gpu_font_text_pool + g_psyq_gpu_font_allocated_characters;
    g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].sprites
        = g_psyq_gpu_font_sprite_pool + g_psyq_gpu_font_allocated_characters;
    *g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].text = 0;
    sprite = g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].sprites;
    for (i = 0; i < characters; i++) {
        SetSprt8(sprite);
        sprite->clut = g_psyq_gpu_font_clut;
        sprite++;
    }
    g_psyq_gpu_font_allocated_characters += characters;
    return g_psyq_gpu_font_window_count++;
}
