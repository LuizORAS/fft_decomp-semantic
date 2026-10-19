#include "psx/libetc.h"

/* The VBlank interrupt's callback: count the VBlank (VSync's count) and call the eight VBlank callbacks in
 * slot order. */
void trapIntrVSync(void) {
    s32 slot = 0;
    psyq_interrupt_callback_t callback;
    Vcount++;
    for (; slot < PSYQ_ETC_CALLBACK_SLOTS; slot++) {
        callback = g_psyq_etc_vblank_callbacks[slot];
        if (callback)
            callback();
    }
}
