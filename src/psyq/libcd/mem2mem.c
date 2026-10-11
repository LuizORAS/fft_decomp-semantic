/* SCUS_942.21 0x80021c5c..0x80021c8f. */
#include "psx/libcd.h"

void mem2mem(u32* destination, u32* source, u32 words, s32 unused_frame_complete) {
    u32 word_index;
    for (word_index = 0; word_index < words; word_index++)
        destination[word_index] = source[word_index];
}
