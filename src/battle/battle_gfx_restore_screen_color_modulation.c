#include "fft/battle_gfx.h"

/* Reload the screen color modulation from the backup battle_gfx_copy_screen_color_modulation saved.
 * No retail code calls it. */
void battle_gfx_restore_screen_color_modulation(void) {
    g_battle_gfx_screen_color_modulation.r = g_battle_gfx_screen_color_modulation_backup.r;
    g_battle_gfx_screen_color_modulation.g = g_battle_gfx_screen_color_modulation_backup.g;
    g_battle_gfx_screen_color_modulation.b = g_battle_gfx_screen_color_modulation_backup.b;
}
