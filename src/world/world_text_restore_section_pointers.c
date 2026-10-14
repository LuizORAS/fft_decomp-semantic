#include "fft/world.h"
#include "psx/types.h"

/* Put back the text section pointers world_text_save_section_pointers backed up and allow the next
 * backup; same as battle_text_restore_section_pointers. */
void world_text_restore_section_pointers(void) {
    world_util_copy_bytes(
        g_world_text_section_pointers, g_world_text_saved_section_pointers, sizeof(g_world_text_section_pointers));
    g_world_text_section_pointers_saved = 0;
}
