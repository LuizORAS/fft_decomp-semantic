#include "fft/world.h"

void world_text_restore_section_pointers(void) {
    world_script_copy_bytes(
        g_text_section_pointers, g_world_text_saved_section_pointers, sizeof(g_text_section_pointers));
    g_world_text_section_pointers_saved = 0;
}
