#include "fft/world.h"
#include "psx/libgpu.h"
#include "psx/types.h"

/* Link a GPU primitive into the secondary ordering table (libgpu addPrim). */
void world_gfx_append_gpu_primitive_to_secondary_otag(u32* prim) {
    u32* entries = g_world_gfx_current_otag_entry;

    addPrim(&entries[1], prim);
}
