#include "psx/libetc.h"

/* Same as psyq_etc_clear_interrupt_state_words: startIntrVSync clears the VBlank callbacks with it, and
 * startIntrDMA the DMA callbacks with psyq_etc_clear_dma_callbacks, a second copy of this text. */
void psyq_etc_clear_vblank_callbacks(psyq_interrupt_callback_t* callbacks, s32 count) {
    s32 i;
    for (i = count - 1; i != -1; i--) {
        *callbacks++ = 0;
    }
}
