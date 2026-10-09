#include "fft/battle.h"
#include "psx/types.h"

void battle_map_init_darkness(void) {
    map_color_t* color;

    color = (map_color_t*)battle_map_light_state_command(MAP_LIGHT_COMMAND_GET_DARKNESS_COLOR, (u8*)color);
    g_battle_map_darkness_color = *color;
    g_battle_map_darkness_transition.channels.red = color->red << 16;
    g_battle_map_darkness_transition.channels.green = color->green << 16;
    g_battle_map_darkness_transition.channels.blue = color->blue << 16;
}
