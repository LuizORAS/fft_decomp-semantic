#include "fft/battle.h"
#include "psx/types.h"

/* Seed the background gradient fade from the gradient the map loaded: the base pair and the 16.16
 * transition channels.
 *
 * The unset `colors` is passed as the command's data argument, which the GET command does not read, and
 * then receives the result. */
void battle_map_init_background_gradient(void) {
    map_background_gradient_colors_t* colors;

    colors = battle_map_light_state_command(MAP_LIGHT_COMMAND_GET_BACKGROUND_GRADIENT, colors);
    g_battle_map_background_gradient_transition.colors[0] = colors->first;
    g_battle_map_background_gradient_transition.colors[1] = colors->second;
    g_battle_map_background_gradient_transition.channels[0].red = colors->first.red << 16;
    g_battle_map_background_gradient_transition.channels[0].green = colors->first.green << 16;
    g_battle_map_background_gradient_transition.channels[0].blue = colors->first.blue << 16;
    g_battle_map_background_gradient_transition.channels[1].red = colors->second.red << 16;
    g_battle_map_background_gradient_transition.channels[1].green = colors->second.green << 16;
    g_battle_map_background_gradient_transition.channels[1].blue = colors->second.blue << 16;
}
