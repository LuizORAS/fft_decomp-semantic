#include "fft/battle_map.h"

/* Add per-component deltas to the map background color and load it as the GTE back color. No
 * retail code calls it.
 *
 * A component whose tested delta is negative (`current + delta < current`) becomes 0xff instead. The
 * retail routine tests green with red_delta. */
void battle_map_adjust_background_color(s16 red_delta, s16 green_delta, s16 blue_delta) {
    if (g_battle_map_background_color.r + red_delta < g_battle_map_background_color.r) {
        g_battle_map_background_color.r = 0xff;
    } else {
        g_battle_map_background_color.r += red_delta;
    }
    if (g_battle_map_background_color.g + red_delta < g_battle_map_background_color.g) {
        g_battle_map_background_color.g = 0xff;
    } else {
        g_battle_map_background_color.g += green_delta;
    }
    if (g_battle_map_background_color.b + blue_delta < g_battle_map_background_color.b) {
        g_battle_map_background_color.b = 0xff;
    } else {
        g_battle_map_background_color.b += blue_delta;
    }
    battle_map_control_gte_background_color(BATTLE_BACKGROUND_COLOR_SET, g_battle_map_background_color.r,
        g_battle_map_background_color.g, g_battle_map_background_color.b);
}
