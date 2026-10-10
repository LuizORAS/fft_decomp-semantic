#include "fft/battle.h"
#include "psx/types.h"

/* Seed the darkness fade from the map's current darkness colour: the base colour
 * and the 16.16 transition channels.
 *
 * The unset `color` is passed as the command's data argument, which the GET
 * command does not read, and then receives the result. */
void battle_map_init_darkness(void) {
    map_color_t* color;

    color = battle_map_light_state_command(MAP_LIGHT_COMMAND_GET_DARKNESS_COLOR, color);
    g_battle_map_darkness_color = *color;
    g_battle_map_darkness_transition.channels.red = color->red << 16;
    g_battle_map_darkness_transition.channels.green = color->green << 16;
    g_battle_map_darkness_transition.channels.blue = color->blue << 16;
}
