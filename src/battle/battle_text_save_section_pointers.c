#include "fft/battle.h"
#include "psx/types.h"

/* Back up the text section pointer table once;
 * battle_text_restore_section_pointers undoes it. */
void battle_text_save_section_pointers(void) {
    if (g_battle_text_section_pointers_saved == 0) {
        battle_util_copy_bytes(g_battle_text_saved_section_pointers, g_battle_text_section_pointers, 0x80);
        g_battle_text_section_pointers_saved++;
    }
}
