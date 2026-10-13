#include "fft/memory.h"
#include "fft/world.h"

void world_text_restore_section_pointers(void) {
    copy_bytes_nonempty(g_text_section_pointers, g_world_text_saved_section_pointers, sizeof(g_text_section_pointers));
    g_world_text_section_pointers_saved = 0;
}
