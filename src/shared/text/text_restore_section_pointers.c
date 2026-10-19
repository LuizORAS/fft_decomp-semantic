#include "fft/memory.h"
#include "fft/menu.h"

/* Put back the text section pointer table battle_text_save_section_pointers
 * backed up and allow the next backup. WORLD compiles this text too. */
void text_restore_section_pointers(void) {
    mem_copy_bytes_nonempty(g_text_section_pointers, g_text_saved_section_pointers, sizeof(g_text_section_pointers));
    g_text_section_pointers_saved = 0;
}
