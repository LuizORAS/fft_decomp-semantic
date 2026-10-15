/* LIBGPU 80024bd8-80024c38. */
#include "psx/libetc.h"
#include "psx/libgpu.h"

/* Wait until drawing is done and send the single primitive `primitive` to GP0 by the CPU.
 * battle_gfx_draw_or_append_gpu_primitive and its WORLD twin use it. */
void DrawPrim(void* primitive) {
    int count = ((P_TAG*)primitive)->len;
    g_psyq_gpu_dispatch->sync(0);
    g_psyq_gpu_dispatch->commands((u32*)primitive + 1, count);
}
