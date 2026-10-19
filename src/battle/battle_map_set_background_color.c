#include "fft/battle_map.h"

/* Set the map background colour and load it as the GTE back colour
 * (battle_map_control_gte_background_color). No retail code calls it. */
void battle_map_set_background_color(s32 red, s32 green, s32 blue) {
    g_battle_map_background_color.r = red;
    g_battle_map_background_color.g = green;
    g_battle_map_background_color.b = blue;
    battle_map_control_gte_background_color(BATTLE_BACKGROUND_COLOR_SET, g_battle_map_background_color.r,
        g_battle_map_background_color.g, g_battle_map_background_color.b);
}
