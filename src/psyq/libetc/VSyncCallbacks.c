#include "psx/libetc.h"

psyq_interrupt_callback_t VSyncCallbacks(s32 slot, psyq_interrupt_callback_t callback) {
    return g_psyq_etc_dispatch->vblank_callback(slot, callback);
}
