#include "fft/battle.h"
#include "fft/battle_menu.h"

/* Copies the normal and the dimmed 16-colour menu palette into the menu palette color table. */
void battle_menu_copy_palette_colors(const menu_palette_pair_t* source) {
    rgba5551_t* destination;

    destination = g_battle_menu_palette_colors.colors;
    battle_copy_bytes(destination, source->colors, sizeof(palette_t));
    battle_copy_bytes(destination + PALETTE_COLOR_COUNT, source->colors + PALETTE_COLOR_COUNT, sizeof(palette_t));
}
