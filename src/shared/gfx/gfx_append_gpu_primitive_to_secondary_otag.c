#include "fft/gfx.h"

/* Link a GPU primitive into the current ordering table entry (libgpu addPrim). */
void gfx_append_gpu_primitive_to_secondary_otag(u32* prim) {
    u32* entries = g_gfx_current_otag_entry;

    addPrim(&entries[1], prim);
}
