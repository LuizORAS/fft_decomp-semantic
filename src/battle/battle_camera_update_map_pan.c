#include "fft/battle.h"

void battle_camera_update_map_pan(void) {
    s32* coord;
    s32 limit;

    if (g_battle_state_game_flow_running != 0) {
        /* The target passes only the mode; the definition's second parameter is a matching device. */
        switch (battle_camera_get_input_direction(BATTLE_CAMERA_INPUT_HELD)) {
        case BATTLE_FACING_NORTH:
            coord = &g_battle_camera_current_real_coords.vz;
            *coord += 0x4000;
            break;
        case BATTLE_FACING_SOUTH:
            coord = &g_battle_camera_current_real_coords.vz;
            *coord -= 0x4000;
            break;
        case BATTLE_FACING_WEST:
            coord = &g_battle_camera_current_real_coords.vx;
            *coord -= 0x4000;
            break;
        case BATTLE_FACING_EAST:
            coord = &g_battle_camera_current_real_coords.vx;
            *coord += 0x4000;
            break;
        case BATTLE_FACING_NORTH_EAST: {
            s32* y = &g_battle_camera_current_real_coords.vz;
            *y += 0x4000;
            g_battle_camera_current_real_coords.vx += 0x4000;
            break;
        }
        case BATTLE_FACING_NORTH_WEST: {
            s32* y = &g_battle_camera_current_real_coords.vz;
            *y += 0x4000;
            g_battle_camera_current_real_coords.vx -= 0x4000;
            break;
        }
        case BATTLE_FACING_SOUTH_EAST: {
            s32* y = &g_battle_camera_current_real_coords.vz;
            *y -= 0x4000;
            g_battle_camera_current_real_coords.vx += 0x4000;
            break;
        }
        case BATTLE_FACING_SOUTH_WEST: {
            s32* y = &g_battle_camera_current_real_coords.vz;
            *y -= 0x4000;
            g_battle_camera_current_real_coords.vx -= 0x4000;
            break;
        }
        }
        {
            s32* x = &g_battle_camera_current_real_coords.vx;
            limit = (g_battle_map_max_x * 0x1C000) + 0x70000;
            if (limit < *x)
                *x = limit;
            if (*x < -0x70000)
                *x = -0x70000;
        }
        limit = (g_battle_map_max_y * 0x1C000) + 0x70000;
        if (limit < g_battle_camera_current_real_coords.vz)
            g_battle_camera_current_real_coords.vz = limit;
        if (g_battle_camera_current_real_coords.vz < -0x70000)
            g_battle_camera_current_real_coords.vz = -0x70000;
    }
}
