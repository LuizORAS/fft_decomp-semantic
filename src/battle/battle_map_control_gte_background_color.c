#include "fft/battle_map.h"

/* Set, read, blank or restore the GTE back color (SetBackColor: the color the GTE adds to every
 * lit polygon), kept in g_battle_map_back_color_red, _green and _blue. The map's lighting resource sets
 * it; the ambient color fades write the same 32-bit values through battle_map_light_state_command.
 *
 * SET stores the three components and loads them into the GTE, and returns the address of the stored red
 * value; while the back color is disabled it does neither and returns 0. GET returns that address.
 * DISABLE loads black and blocks SET; RESTORE reloads the stored color and lifts the block.
 *
 * DISABLE, RESTORE and a command with no case return the unset `result`. Every caller issues SET and
 * ignores the result. */
s32* battle_map_control_gte_background_color(s32 command, s32 red, s32 green, s32 blue) {
    /* Left unset on the DISABLE, RESTORE and default paths. */
    s32* result;

    switch (command) {
    case BATTLE_BACKGROUND_COLOR_SET: {
        s32* stored_values;

        if (g_battle_map_back_color_disabled == 1) {
            return 0;
        }
        stored_values = &g_battle_map_back_color_red;
        *stored_values = red;
        g_battle_map_back_color_green = green;
        g_battle_map_back_color_blue = blue;
        SetBackColor(red, green, blue);
        return stored_values;
    }
    case BATTLE_BACKGROUND_COLOR_GET:
        result = &g_battle_map_back_color_red;
        break;
    case BATTLE_BACKGROUND_COLOR_DISABLE:
        SetBackColor(0, 0, 0);
        g_battle_map_back_color_disabled = 1;
        return result;
    case BATTLE_BACKGROUND_COLOR_RESTORE:
        SetBackColor(g_battle_map_back_color_red, g_battle_map_back_color_green, g_battle_map_back_color_blue);
        g_battle_map_back_color_disabled = 0;
        return result;
    }
    return result;
}
