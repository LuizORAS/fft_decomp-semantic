#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "fft/battle_map.h"

/* Pans the camera with the directional buttons, once per frame of the battle
 * loop, while g_camera_map_pan_enabled is set (the states that put the
 * buttons in free-cursor mode: unit highlighting, move-area display and ability
 * preview).
 *
 * Each held direction moves the x or map-depth coordinate by 0x4000 (four world
 * units in 20.12) per frame, diagonals both. The position is then clamped to four
 * tiles (0x70000) outside the map on every side: -0x70000 up to
 * g_map_max_x or g_map_max_y tiles of 0x1c000 plus 0x70000. */
void camera_update_map_pan(void) {
    s32* coord;
    s32 limit;

    if (g_camera_map_pan_enabled != 0) {
        switch (camera_get_input_direction(CAMERA_INPUT_HELD)) {
        case BATTLE_FACING_NORTH:
            coord = &g_camera_current_real_coords.vz;
            *coord += 0x4000;
            break;
        case BATTLE_FACING_SOUTH:
            coord = &g_camera_current_real_coords.vz;
            *coord -= 0x4000;
            break;
        case BATTLE_FACING_WEST:
            coord = &g_camera_current_real_coords.vx;
            *coord -= 0x4000;
            break;
        case BATTLE_FACING_EAST:
            coord = &g_camera_current_real_coords.vx;
            *coord += 0x4000;
            break;
        case BATTLE_FACING_NORTH_EAST: {
            s32* y = &g_camera_current_real_coords.vz;
            *y += 0x4000;
            g_camera_current_real_coords.vx += 0x4000;
            break;
        }
        case BATTLE_FACING_NORTH_WEST: {
            s32* y = &g_camera_current_real_coords.vz;
            *y += 0x4000;
            g_camera_current_real_coords.vx -= 0x4000;
            break;
        }
        case BATTLE_FACING_SOUTH_EAST: {
            s32* y = &g_camera_current_real_coords.vz;
            *y -= 0x4000;
            g_camera_current_real_coords.vx += 0x4000;
            break;
        }
        case BATTLE_FACING_SOUTH_WEST: {
            s32* y = &g_camera_current_real_coords.vz;
            *y -= 0x4000;
            g_camera_current_real_coords.vx -= 0x4000;
            break;
        }
        }
        {
            s32* x = &g_camera_current_real_coords.vx;
            limit = (g_map_max_x * 0x1C000) + 0x70000;
            if (limit < *x)
                *x = limit;
            if (*x < -0x70000)
                *x = -0x70000;
        }
        limit = (g_map_max_y * 0x1C000) + 0x70000;
        if (limit < g_camera_current_real_coords.vz)
            g_camera_current_real_coords.vz = limit;
        if (g_camera_current_real_coords.vz < -0x70000)
            g_camera_current_real_coords.vz = -0x70000;
    }
}
