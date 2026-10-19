#include "fft/gfx.h"

/*
 * Submit a primitive for the active rendering mode: drawn at once in immediate mode (BATTLE's ETC
 * full-screen graphics), otherwise linked into the current ordering-table entry (libgpu addPrim).
 */
void gfx_draw_or_append_gpu_primitive(void* primitive) {
    if (g_gfx_draw_primitives_immediately != 0) {
        DrawPrim(primitive);
        return;
    }
    addPrim(g_gfx_current_otag_entry, primitive);
}
