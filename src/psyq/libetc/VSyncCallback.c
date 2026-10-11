#include "psx/libetc.h"

void VSyncCallback(psyq_interrupt_callback_t callback) {
    g_psyq_etc_dispatch->vblank_callback(0, callback);
}
