#include "fft/battle_map.h"

void battle_map_restore_background_color(void) {
    g_battle_map_background_color.r = g_battle_map_background_color_backup.r;
    g_battle_map_background_color.g = g_battle_map_background_color_backup.g;
    g_battle_map_background_color.b = g_battle_map_background_color_backup.b;
    battle_map_control_gte_background_color(BATTLE_BACKGROUND_COLOR_SET, g_battle_map_background_color.r,
        g_battle_map_background_color.g, g_battle_map_background_color.b);
}
