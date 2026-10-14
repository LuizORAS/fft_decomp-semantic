#include "fft/world.h"
#include "psx/types.h"

/* Point all 32 text sections at the menu text WORLD.BIN carries, a copy of EVENT/SAMPLE.LZW
 * (g_world_text_section_offsets, counted from the end of the table), as battle_text_init_menu_section_pointers
 * does for BATTLE but keeping no section. Its callers are WORLD's copies of the battle menu routines
 * (world_menu_init_palette_and_text, world_menu_run_system_function_thread, world_text_build_unit_name_list). */
void world_text_init_section_pointers(void) {
    s32* offset;
    u8** section;
    s32 i;
    u8* text;
    s32 value;

    offset = g_world_text_section_offsets;
    i = 0;
    text = (u8*)(offset + TEXT_SECTION_COUNT);
    section = g_world_text_section_pointers;
    do {
        value = *offset;
        offset++;
        i++;
        value += (s32)text;
        *section = (u8*)value;
        section++;
    } while (i < TEXT_SECTION_COUNT);
}
