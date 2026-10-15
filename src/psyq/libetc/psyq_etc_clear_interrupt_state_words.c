#include "psx/types.h"

/* Clear `count` words at `words`. startIntr clears its state with it;
 * psyq_etc_clear_vblank_callback_words and psyq_etc_clear_dma_callback_words are the same loop. */
void psyq_etc_clear_interrupt_state_words(u32* words, s32 count) {
    s32 i;
    for (i = count - 1; i != -1; i--) {
        *words++ = 0;
    }
}
