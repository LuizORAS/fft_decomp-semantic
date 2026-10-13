#include "fft/gfx.h"

/*
 * Submit a primitive for the active rendering mode.
 *
 * Immediate rendering draws directly. Deferred rendering links
 * the primitive into the current ordering-table entry.
 */
void gfx_draw_or_append_gpu_primitive(void* primitive) {
    if (g_gfx_draw_primitives_immediately != 0) {
        DrawPrim(primitive);
        return;
    }
    addPrim(g_gfx_current_otag_entry, primitive);
}
