#include "fft/world.h"
#include "psx/types.h"

/* Converts the text file's section offset table into absolute section pointers. */
void world_text_relocate_pointer_table(s32* offsets) {
    s32 i = 0;
    u8* base = (u8*)offsets + WORLD_TEXT_FILE_HEADER_BYTES;
    u8** dst = g_text_section_pointers;
    for (; i < TEXT_SECTION_COUNT; i++) {
        *dst++ = base + *offsets++;
    }
}
