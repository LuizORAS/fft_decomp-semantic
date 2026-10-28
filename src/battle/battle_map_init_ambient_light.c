#include "fft/battle_map.h"

/* Seed the ambient light fade from the color the map loaded: the base color and
 * the 16.16 transition channels take the current GTE back color.
 *
 * The unset `color` is passed as the command's data argument, which the GET
 * command does not read, and then receives the result. */
void battle_map_init_ambient_light(void) {
    CVECTOR* color;

    color = battle_map_light_state_command(MAP_LIGHT_COMMAND_GET_AMBIENT_COLOR, color);
    g_battle_map_ambient_light_color = *color;
    g_battle_map_ambient_light_transition.channels.red = color->r << 16;
    g_battle_map_ambient_light_transition.channels.green = color->g << 16;
    g_battle_map_ambient_light_transition.channels.blue = color->b << 16;
}
