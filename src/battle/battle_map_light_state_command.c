#include "fft/battle.h"
#include "psx/types.h"

/* Map colour state commands, selected on command - 0x56: 0x56 applies the
 * background gradient pair, 0x5a the ambient colour and 0x63 the darkness
 * colour; the neighbouring commands save, restore or return those records.
 *
 * Only the GET arms assign `result`; the others return it uninitialized, as
 * the target does. */
void* battle_map_light_state_command(s32 command, void* data) {
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
        g_battle_map_current_darkness_color.r = ((CVECTOR*)data)->r;
        g_battle_map_current_darkness_color.g = ((CVECTOR*)data)->g;
        g_battle_map_current_darkness_color.b = ((CVECTOR*)data)->b;
        break;
    case MAP_LIGHT_COMMAND_RESET_DARKNESS_COLOR:
        g_battle_map_current_darkness_color.r = 0;
        g_battle_map_current_darkness_color.g = 0;
        g_battle_map_current_darkness_color.b = 1;
        break;
    case MAP_LIGHT_COMMAND_GET_DARKNESS_COLOR:
        result = &g_battle_map_current_darkness_color;
        break;
    }
    return result;
}
