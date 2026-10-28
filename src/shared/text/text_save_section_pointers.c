#include "fft/memory.h"
#include "fft/menu.h"

/* Back up the text section pointer table once;
 * battle_text_restore_section_pointers undoes it. WORLD compiles this text too. */
void text_save_section_pointers(void) {
    if (g_text_section_pointers_saved == 0) {
        mem_copy_bytes_nonempty(
            g_text_saved_section_pointers, g_text_section_pointers, sizeof(g_text_saved_section_pointers));
        g_text_section_pointers_saved++;
    }
}
