#include "fft/memory.h"
#include "fft/menu.h"

/* Back up the text section pointer table once. */
void text_save_section_pointers(void) {
    if (g_text_section_backup_active == 0) {
        mem_copy_bytes_nonempty(
            g_text_saved_section_pointers, g_text_section_pointers, sizeof(g_text_saved_section_pointers));
        g_text_section_backup_active++;
    }
}
