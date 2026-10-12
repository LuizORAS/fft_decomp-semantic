#include "fft/battle.h"
#include "fft/battle_text.h"

/* Put back the text section pointer table battle_text_save_section_pointers
 * backed up and allow the next backup. */
void battle_text_restore_section_pointers(void) {
    battle_copy_bytes(g_text_section_pointers, g_battle_text_pointer_backup, sizeof(g_text_section_pointers));
    g_battle_text_pointer_backup_active = 0;
}
