#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Advances the screen-offset move by one frame while its countdown runs. Only the
 * uncalled camera_set_offset_screen_coord_delta_* routines start the
 * countdown, so nothing moves in retail. */
void camera_update_offset_screen_coord_animation(void) {
    s32 fixed_x;
    s32 fixed_z;
    s32 fixed_y;

    if (g_camera_offset_screen_coord_countdown != 0) {
        fixed_x = g_camera_offset_screen_coords_fixed.vx + g_camera_real_coord_delta.vx;
        g_camera_offset_screen_coords_fixed.vx = fixed_x;
        fixed_z = g_camera_offset_screen_coords_fixed.vy + g_camera_real_coord_delta.vy;
        fixed_y = g_camera_offset_screen_coords_fixed.vz + g_camera_real_coord_delta.vz;
        g_camera_offset_screen_coords_fixed.vy = fixed_z;
        g_camera_offset_screen_coords_fixed.vz = fixed_y;
        setVector(&g_battle_offset_screen_coords, fixed_x / ONE, fixed_z / ONE, fixed_y / ONE);
        g_camera_offset_screen_coord_countdown--;
    }
}
