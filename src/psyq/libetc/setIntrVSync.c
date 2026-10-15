#include "psx/libetc.h"

/* Set the VBlank callback in slot `slot` (0-7) and return the previous one. VSyncCallback and
 * VSyncCallbacks reach it through the dispatch table. */
psyq_interrupt_callback_t setIntrVSync(s32 slot, psyq_interrupt_callback_t callback) {
    psyq_interrupt_callback_t previous = g_psyq_etc_vblank_callbacks[slot];
    if (callback != previous)
        g_psyq_etc_vblank_callbacks[slot] = callback;
    return previous;
}
