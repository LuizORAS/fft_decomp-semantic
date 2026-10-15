#include "psx/types.h"

/* Same as psyq_etc_clear_interrupt_state_words; startIntrVSync clears the VBlank callbacks with it. */
void psyq_etc_clear_vblank_callback_words(u32* words, s32 count) {
    s32 i;
    for (i = count - 1; i != -1; i--) {
        *words++ = 0;
    }
}
