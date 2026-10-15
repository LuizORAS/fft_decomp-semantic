/* LIBGPU 80022f2c-80022fd0. */
#include "psx/libc.h"
#include "psx/libgpu.h"

/* Load the built-in debug font to VRAM: its 16-colour CLUT at (`x`, `y` + 128) and its 4-bit texture page at
 * (`x`, `y`), then close every font window. main_gfx_load_efc_fnt and main_gfx_reset_display load it (the
 * first patching this routine's CLUT row, QUIRKS.md). */
void FntLoad(int x, int y) {
    u8* data = g_psyq_gpu_font_asset;
    g_psyq_gpu_font_clut = LoadClut2((u32*)data, x, y + 128);
    g_psyq_gpu_font_tpage = LoadTPage((u32*)(data + 512), 0, 0, x, y, 128, 32);
    g_psyq_gpu_font_window_count = 0;
    memset(g_psyq_gpu_font_windows, 0, 384);
}
