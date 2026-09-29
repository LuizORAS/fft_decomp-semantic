#include "psx/libetc.h"

void trapIntrVSync(void) {
    s32 slot = 0;
    psyq_interrupt_callback_t* callbacks = g_psyq_etc_vblank_callbacks;
    psyq_interrupt_callback_t callback;
    g_psyq_etc_vblank_count++;
    for (; slot < PSYQ_ETC_CALLBACK_SLOTS; slot++) {
        callback = *callbacks++;
        if (callback)
            callback();
    }
}
