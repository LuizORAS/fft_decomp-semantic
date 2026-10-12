#include "fft/world.h"

/* WORLD twin of battle_menu_init_palette_and_text; the argument is not read. */
void world_menu_init_palette_and_text(s32 unused) {
    world_menu_copy_palette_colors(g_world_menu_default_palette_colors);
    world_text_init_section_pointers();
}
