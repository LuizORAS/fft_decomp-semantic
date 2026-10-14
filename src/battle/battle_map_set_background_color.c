#include "fft/battle.h"

/* Set the map background colour and load it as the GTE back colour
 * (battle_map_control_gte_background_color). No retail code calls it. */
void battle_map_set_background_color(s32 red, s32 green, s32 blue) {
    g_battle_map_background_color[0] = red;
    g_battle_map_background_color[1] = green;
    g_battle_map_background_color[2] = blue;
    battle_map_control_gte_background_color(BATTLE_BACKGROUND_COLOR_SET, g_battle_map_background_color[0],
        g_battle_map_background_color[1], g_battle_map_background_color[2]);
}
