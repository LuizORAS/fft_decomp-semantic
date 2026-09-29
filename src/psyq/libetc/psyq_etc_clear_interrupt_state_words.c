#include "psx/types.h"
void psyq_etc_clear_interrupt_state_words(u32* words, s32 count) {
    /* Retain the original eight-byte frame; removing it shortens the body. */
    u32 retained_frame[2];
    /* Unpinned loop state reuses a1 instead of the original v0. */
    register s32 remaining __asm__("$2");
    /* Prevent reorg from moving the stack adjustment into the branch delay slot. */
    __asm__ volatile("");
    if (count != 0) {
        remaining = count - 1;

        do {
            *words++ = 0;
        } while (--remaining != -1);
    }
}
