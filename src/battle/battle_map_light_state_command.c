#include "fft/battle.h"
#include "psx/types.h"

/* Set, read, save or restore the map's three colour records, selected on command - 0x56: the
 * background gradient pair (0x56-0x59), the ambient colour, which is the GTE back colour (0x5a-0x5d), and
 * the darkness colour, the colour of the untextured map polygons (0x63 sets it, 0x64 resets it to 0/0/1,
 * 0x65 returns it).
 *
 * Only the GET arms assign `result`; the others return it uninitialized, as the target does. */
void* battle_map_light_state_command(s32 command, void* data) {
    void* result;

    switch (command) {
    case MAP_LIGHT_COMMAND_SET_BACKGROUND_GRADIENT:
        g_battle_map_background_gradient_colors.first = ((map_color_t*)data)[0];
        g_battle_map_background_gradient_colors.second = ((map_color_t*)data)[1];
        break;
    case MAP_LIGHT_COMMAND_GET_BACKGROUND_GRADIENT:
        result = &g_battle_map_background_gradient_colors;
        break;
    case MAP_LIGHT_COMMAND_SAVE_BACKGROUND_GRADIENT:
        g_battle_map_saved_background_gradient_first = g_battle_map_background_gradient_colors.first;
        g_battle_map_saved_background_gradient_second = g_battle_map_background_gradient_colors.second;
        break;
    case MAP_LIGHT_COMMAND_RESTORE_BACKGROUND_GRADIENT:
        g_battle_map_background_gradient_colors.first = g_battle_map_saved_background_gradient_first;
        g_battle_map_background_gradient_colors.second = g_battle_map_saved_background_gradient_second;
        break;
    case MAP_LIGHT_COMMAND_SET_AMBIENT_COLOR:
        g_battle_map_back_color_red = ((map_color_t*)data)->red;
        g_battle_map_back_color_green = ((map_color_t*)data)->green;
        g_battle_map_back_color_blue = ((map_color_t*)data)->blue;
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
        u8* packed = g_battle_map_back_color_bytes;

        packed[0] = g_battle_map_back_color_red;
        g_battle_map_back_color_bytes[1] = g_battle_map_back_color_green;
        g_battle_map_back_color_bytes[2] = g_battle_map_back_color_blue;
        result = packed;
        break;
    }
    case MAP_LIGHT_COMMAND_SET_DARKNESS_COLOR:
        g_battle_map_untextured_polygon_color[0] = ((map_color_t*)data)->red;
        g_battle_map_untextured_polygon_color[1] = ((map_color_t*)data)->green;
        g_battle_map_untextured_polygon_color[2] = ((map_color_t*)data)->blue;
        break;
    case MAP_LIGHT_COMMAND_RESET_DARKNESS_COLOR:
        g_battle_map_untextured_polygon_color[0] = 0;
        g_battle_map_untextured_polygon_color[1] = 0;
        g_battle_map_untextured_polygon_color[2] = 1;
        break;
    case MAP_LIGHT_COMMAND_GET_DARKNESS_COLOR:
        result = g_battle_map_untextured_polygon_color;
        break;
    }
    return result;
}
