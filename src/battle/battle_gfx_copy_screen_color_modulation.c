#include "fft/battle.h"

/* Saves the screen colour modulation for
 * battle_gfx_restore_screen_color_modulation. No retail code calls either. */
void battle_gfx_copy_screen_color_modulation(void) {
    g_battle_gfx_screen_color_modulation_backup.r = g_battle_gfx_screen_color_modulation.r;
    g_battle_gfx_screen_color_modulation_backup.g = g_battle_gfx_screen_color_modulation.g;
    g_battle_gfx_screen_color_modulation_backup.b = g_battle_gfx_screen_color_modulation.b;
}
