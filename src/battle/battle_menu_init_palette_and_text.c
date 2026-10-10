#include "fft/battle.h"
#include "psx/types.h"

/* Load the default menu palette and bind the menu text sections. The caller
 * supplies an argument nothing reads. */
void battle_menu_init_palette_and_text(s32 unused_0) {
    battle_menu_copy_palette_colors(g_battle_menu_default_palette_colors);
    battle_text_init_menu_section_pointers();
}
