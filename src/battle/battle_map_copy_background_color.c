#include "fft/battle_map.h"

/* Save the map background colour in the backup that
 * battle_map_restore_background_color reloads. */
void battle_map_copy_background_color(void) {
    /* Component copies retain the original byte loads and separate addresses. */
    g_battle_map_background_color_backup.r = g_battle_map_background_color.r;
    g_battle_map_background_color_backup.g = g_battle_map_background_color.g;
    g_battle_map_background_color_backup.b = g_battle_map_background_color.b;
}
