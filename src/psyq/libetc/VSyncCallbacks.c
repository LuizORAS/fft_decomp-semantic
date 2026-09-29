#include "psx/libetc.h"

void* VSyncCallbacks(s32 slot, void* callback) {
    return g_psyq_etc_dispatch->vblank_callback(slot, callback);
}
