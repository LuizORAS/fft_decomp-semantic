/* LIBGPU 80022fd0-80023288. */
#include "psx/libgpu.h"

int FntOpen(int x, int y, int width, int height, int background, int characters) {
    RECT window;
    SPRT_8* sprite;
    /* The tile base and later temporary retain distinct original registers. */
    DR_MODE* mode_base;
    register psyq_font_window_t* tile_base __asm__("$16");
    int i;
    int number;
    int total;
    register psyq_font_window_t* current __asm__("$2");
    if (g_psyq_gpu_font_window_count >= 8)
        return -1;
    if (g_psyq_gpu_font_window_count == 0)
        g_psyq_gpu_font_allocated_characters = 0;
    g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].wrap_disabled = width == 0;
    if (characters + g_psyq_gpu_font_allocated_characters > 1024)
        characters = 1024 - g_psyq_gpu_font_allocated_characters;
    mode_base = &g_psyq_gpu_font_windows[0].mode;
    window.w = 256;
    window.h = 256;
    window.x = 0;
    window.y = 0;
    SetDrawMode(&g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].mode, 0, 0, g_psyq_gpu_font_tpage, &window);
    /* Keep the retail's distinct tile and draw-mode bases across its calls. */
    __asm__("" : "=r"(mode_base) : "0"(mode_base));
    tile_base = (psyq_font_window_t*)((TILE*)mode_base - 1);
    if (background) {
        /* Integer address addition retains the retail operand order. */
        SetTile(
            &((psyq_font_window_t*)((u32)(g_psyq_gpu_font_window_count * sizeof(*tile_base)) + (u32)tile_base))->tile);
        tile_base[g_psyq_gpu_font_window_count].tile.r0 = 0;
        tile_base[g_psyq_gpu_font_window_count].tile.g0 = 0;
        tile_base[g_psyq_gpu_font_window_count].tile.b0 = 0;
        SetSemiTrans(
            &((psyq_font_window_t*)((u32)(g_psyq_gpu_font_window_count * sizeof(*tile_base)) + (u32)tile_base))->tile,
            background == 2);
    }
    __asm__("" : "=r"(mode_base) : "0"(mode_base));
    number = g_psyq_gpu_font_window_count;
    current = (psyq_font_window_t*)((TILE*)mode_base - 1);
    /* Typed pointer addition reverses the retail addu operands. */
    current = (psyq_font_window_t*)((u32)(number * sizeof(*current)) + (u32)current);
    total = g_psyq_gpu_font_allocated_characters;
    current->tile.x0 = x;
    current->tile.y0 = y;
    current->tile.w = width;
    current->tile.h = height;
    g_psyq_gpu_font_windows[number].character_limit = characters;
    g_psyq_gpu_font_windows[number].text_count = 0;
    g_psyq_gpu_font_windows[number].text = g_psyq_gpu_font_text_pool + total;
    g_psyq_gpu_font_windows[number].sprites = g_psyq_gpu_font_sprite_pool + total;
    *g_psyq_gpu_font_windows[number].text = 0;
    sprite = g_psyq_gpu_font_windows[g_psyq_gpu_font_window_count].sprites;
    for (i = 0; i < characters; i++) {
        SetSprt8(sprite);
        sprite->clut = g_psyq_gpu_font_clut;
        sprite++;
    }
    g_psyq_gpu_font_allocated_characters += characters;
    return g_psyq_gpu_font_window_count++;
}
