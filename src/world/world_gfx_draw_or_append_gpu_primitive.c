#include "fft/world.h"
#include "psx/libgpu.h"

/* Draw the primitive at once, or link it into the active ordering-table entry (libgpu addPrim). */
void world_gfx_draw_or_append_gpu_primitive(void* primitive) {
    if (g_world_gfx_draw_primitives_immediately != 0) {
        DrawPrim(primitive);
        return;
    }
    addPrim(g_world_gfx_active_otag_entries, primitive);
}
