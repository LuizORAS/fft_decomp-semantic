#include "fft/battle_text.h"
#include "fft/memory.h"
#include "fft/menu.h"

/* Back up the text section pointer table once. */
void text_save_section_pointers(void) {
    if (g_battle_text_pointer_backup_active == 0) {
        mem_copy_bytes_nonempty(
            g_battle_text_pointer_backup, g_text_section_pointers, sizeof(g_battle_text_pointer_backup));
        g_battle_text_pointer_backup_active++;
    }
}
