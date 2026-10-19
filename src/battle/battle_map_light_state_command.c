#include "fft/battle_map.h"
#include "psx/types.h"

/* Set, read, save or restore the map's three colour records, selected on command - 0x56: the
 * background gradient pair (0x56-0x59), the ambient colour, which is the GTE back colour (0x5a-0x5d), and
 * the darkness colour, the colour of the untextured map polygons (0x63 sets it, 0x64 resets it to 0/0/1,
 * 0x65 returns it).
 *
 * Only the GET arms assign `result`; the others return it uninitialized, as the target does. */
void* battle_map_light_state_command(map_light_command_e command, void* data) {
    void* result;

    switch (command) {
    case MAP_LIGHT_COMMAND_SET_BACKGROUND_GRADIENT:
        g_battle_map_background_gradient_colors.colors[MAP_BACKGROUND_GRADIENT_TOP]
            = ((map_background_gradient_colors_t*)data)->colors[MAP_BACKGROUND_GRADIENT_TOP];
        g_battle_map_background_gradient_colors.colors[MAP_BACKGROUND_GRADIENT_BOTTOM]
            = ((map_background_gradient_colors_t*)data)->colors[MAP_BACKGROUND_GRADIENT_BOTTOM];
        break;
    case MAP_LIGHT_COMMAND_GET_BACKGROUND_GRADIENT:
        result = &g_battle_map_background_gradient_colors;
        break;
    case MAP_LIGHT_COMMAND_SAVE_BACKGROUND_GRADIENT:
        g_battle_map_saved_background_gradient_first
            = g_battle_map_background_gradient_colors.colors[MAP_BACKGROUND_GRADIENT_TOP];
        g_battle_map_saved_background_gradient_second
            = g_battle_map_background_gradient_colors.colors[MAP_BACKGROUND_GRADIENT_BOTTOM];
        break;
    case MAP_LIGHT_COMMAND_RESTORE_BACKGROUND_GRADIENT:
        g_battle_map_background_gradient_colors.colors[MAP_BACKGROUND_GRADIENT_TOP]
            = g_battle_map_saved_background_gradient_first;
        g_battle_map_background_gradient_colors.colors[MAP_BACKGROUND_GRADIENT_BOTTOM]
            = g_battle_map_saved_background_gradient_second;
        break;
    case MAP_LIGHT_COMMAND_SET_AMBIENT_COLOR:
        g_battle_map_back_color_red = ((CVECTOR*)data)->r;
        g_battle_map_back_color_green = ((CVECTOR*)data)->g;
        g_battle_map_back_color_blue = ((CVECTOR*)data)->b;
        SetBackColor(g_battle_map_back_color_red, g_battle_map_back_color_green, g_battle_map_back_color_blue);
        break;
    case MAP_LIGHT_COMMAND_SAVE_AMBIENT_COLOR:
        g_battle_map_saved_back_color_red = g_battle_map_back_color_red;
        g_battle_map_saved_back_color_green = g_battle_map_back_color_green;
        g_battle_map_saved_back_color_blue = g_battle_map_back_color_blue;
        break;
    case MAP_LIGHT_COMMAND_RESTORE_AMBIENT_COLOR:
        g_battle_map_back_color_red = g_battle_map_saved_back_color_red;
        g_battle_map_back_color_green = g_battle_map_saved_back_color_green;
        g_battle_map_back_color_blue = g_battle_map_saved_back_color_blue;
        break;
    case MAP_LIGHT_COMMAND_GET_AMBIENT_COLOR: {
        CVECTOR* color = &g_battle_map_cached_back_color;

        color->r = g_battle_map_back_color_red;
        g_battle_map_cached_back_color.g = g_battle_map_back_color_green;
        g_battle_map_cached_back_color.b = g_battle_map_back_color_blue;
        result = color;
        break;
    }
    case MAP_LIGHT_COMMAND_SET_DARKNESS_COLOR:
        g_battle_map_untextured_polygon_color.r = ((CVECTOR*)data)->r;
        g_battle_map_untextured_polygon_color.g = ((CVECTOR*)data)->g;
        g_battle_map_untextured_polygon_color.b = ((CVECTOR*)data)->b;
        break;
    case MAP_LIGHT_COMMAND_RESET_DARKNESS_COLOR:
        g_battle_map_untextured_polygon_color.r = 0;
        g_battle_map_untextured_polygon_color.g = 0;
        g_battle_map_untextured_polygon_color.b = 1;
        break;
    case MAP_LIGHT_COMMAND_GET_DARKNESS_COLOR:
        result = &g_battle_map_untextured_polygon_color;
        break;
    }
    return result;
}
