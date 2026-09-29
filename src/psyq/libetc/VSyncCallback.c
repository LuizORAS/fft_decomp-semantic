#include "psx/libetc.h"

void VSyncCallback(void* callback) {
    g_psyq_etc_dispatch->vblank_callback(0, callback);
}
