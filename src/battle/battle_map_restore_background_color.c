#include "fft/battle_map.h"

/* Reload the map background color from the backup battle_map_copy_background_color saved and load
 * it as the GTE back color. No retail code calls it. */
void battle_map_restore_background_color(void) {
    g_battle_map_background_color.r = g_battle_map_background_color_backup.r;
    g_battle_map_background_color.g = g_battle_map_background_color_backup.g;
    g_battle_map_background_color.b = g_battle_map_background_color_backup.b;
    battle_map_control_gte_background_color(BATTLE_BACKGROUND_COLOR_SET, g_battle_map_background_color.r,
        g_battle_map_background_color.g, g_battle_map_background_color.b);
}
