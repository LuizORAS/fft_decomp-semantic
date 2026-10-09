#include "psx/types.h"
void psyq_etc_clear_dma_callback_words(u32* words, s32 count) {
    s32 i;
    for (i = count - 1; i != -1; i--) {
        *words++ = 0;
    }
}
