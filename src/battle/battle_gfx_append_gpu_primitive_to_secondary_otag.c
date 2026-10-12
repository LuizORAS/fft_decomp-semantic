#include "fft/battle_action.h"
#include "fft/battle_gfx.h"
#include "psx/libgpu.h"

/* Link a GPU primitive into the current ordering table entry (libgpu addPrim). */
void battle_gfx_append_gpu_primitive_to_secondary_otag(u32* prim) {
    u32* entries = g_battle_gfx_current_otag_entry;

    addPrim(&entries[1], prim);
}
