#include "psx/types.h"

/* Same as psyq_etc_clear_interrupt_state_words; startIntrDMA clears the DMA callbacks with it. */
void psyq_etc_clear_dma_callback_words(u32* words, s32 count) {
    s32 i;
    for (i = count - 1; i != -1; i--) {
        *words++ = 0;
    }
}
