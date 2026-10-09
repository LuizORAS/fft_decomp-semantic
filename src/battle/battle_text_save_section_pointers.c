#include "fft/battle.h"
#include "psx/types.h"

void battle_text_save_section_pointers(void) {
    if (g_battle_text_pointer_backup_active == 0) {
        battle_copy_bytes(g_battle_text_pointer_backup, g_battle_text_section_pointers, 0x80);
        g_battle_text_pointer_backup_active++;
    }
}
