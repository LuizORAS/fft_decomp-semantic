#include "fft/battle_camera.h"
#include "fft/battle_effect.h"

/* Seed the four effect camera snapshots from the live camera.
 *
 * Each snapshot receives the rotation masked to 12 bits, the current real
 * coordinates in whole units (Q12 shifted down), and the camera zoom vector. */
void effect_copy_camera_angles_and_world_coords(void) {
    SVECTOR* rotation;
    VECTOR* coords;

    rotation = camera_get_rotation();
    g_effect_camera_rotation_saved.vx = g_effect_camera_rotation_start.vx = g_effect_camera_rotation_target.vx
        = g_effect_camera_rotation_current.vx = rotation->vx & 0xfff;
    g_effect_camera_rotation_saved.vy = g_effect_camera_rotation_start.vy = g_effect_camera_rotation_target.vy
        = g_effect_camera_rotation_current.vy = rotation->vy & 0xfff;
    g_effect_camera_rotation_saved.vz = g_effect_camera_rotation_start.vz = g_effect_camera_rotation_target.vz
        = g_effect_camera_rotation_current.vz = rotation->vz & 0xfff;
    coords = camera_get_current_real_coords();
    g_effect_camera_position_saved.vx = g_effect_camera_position_start.vx = g_effect_camera_position_target.vx
        = g_effect_camera_position_current.vx = coords->vx >> 12;
    g_effect_camera_position_saved.vy = g_effect_camera_position_start.vy = g_effect_camera_position_target.vy
        = g_effect_camera_position_current.vy = coords->vy >> 12;
    g_effect_camera_position_saved.vz = g_effect_camera_position_start.vz = g_effect_camera_position_target.vz
        = g_effect_camera_position_current.vz = coords->vz >> 12;
    g_effect_camera_zoom_saved = g_effect_camera_zoom_start = g_effect_camera_zoom_target = g_effect_camera_zoom_current
        = *camera_get_zoom();
}
