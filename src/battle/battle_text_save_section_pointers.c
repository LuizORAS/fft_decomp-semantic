#include "fft/battle.h"
#include "fft/battle_text.h"

/* Back up the text section pointer table once;
 * battle_text_restore_section_pointers undoes it. */
void battle_text_save_section_pointers(void) {
    if (g_battle_text_pointer_backup_active == 0) {
        battle_copy_bytes(g_battle_text_pointer_backup, g_text_section_pointers, sizeof(g_battle_text_pointer_backup));
        g_battle_text_pointer_backup_active++;
    }
}
