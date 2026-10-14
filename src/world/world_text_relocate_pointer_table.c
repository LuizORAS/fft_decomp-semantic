#include "fft/world.h"
#include "psx/types.h"

/* Point all 32 text sections at a text file in memory, whose table of 32 section offsets is `offsets`,
 * counted from the end of the table. WORLD binds the reloaded EVENT/WORLD.LZW this way
 * (world_menu_reset_context), and the help boxes, WLDCORE's message windows and OPEN's messages bind
 * EVENT/WLDHELP.LZW (g_world_text_wldhelp_section_offsets) between world_text_save_section_pointers and
 * world_text_restore_section_pointers. */
void world_text_relocate_pointer_table(s32* offsets) {
    s32 i = 0;
    u8* base = (u8*)offsets + WORLD_TEXT_FILE_HEADER_BYTES;
    u8** dst = g_world_text_section_pointers;
    do {
        *dst++ = base + *offsets++;
        i++;
    } while (i < TEXT_SECTION_COUNT);
}
