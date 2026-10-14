#include "fft/battle_text.h"
#include "fft/memory.h"
#include "fft/menu.h"

/* Restore the text section pointer table and clear the backup flag. */
void text_restore_section_pointers(void) {
    mem_copy_bytes_nonempty(g_text_section_pointers, g_battle_text_pointer_backup, sizeof(g_text_section_pointers));
    g_battle_text_pointer_backup_active = 0;
}
