#include "fft/memory.h"
#include "fft/menu.h"

/* Restore the text section pointer table and clear the backup flag. */
void text_restore_section_pointers(void) {
    mem_copy_bytes_nonempty(g_text_section_pointers, g_text_saved_section_pointers, sizeof(g_text_section_pointers));
    g_text_section_backup_active = 0;
}
