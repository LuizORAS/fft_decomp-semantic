#include "psx/libetc.h"

/* Set the VBlank callback in slot `slot` (0-7) and return the previous one. No code refers to it. */
void* VSyncCallbacks(s32 slot, void* callback) {
    return g_psyq_etc_dispatch->vblank_callback(slot, callback);
}
