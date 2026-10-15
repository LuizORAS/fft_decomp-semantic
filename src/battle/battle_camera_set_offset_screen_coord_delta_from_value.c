#include "fft/battle.h"

/* Starts a linear move of all three screen-offset components to the one value
 * *destination_value over frame_count frames (divided by the animation speed).
 * battle_camera_update_offset_screen_coord_animation applies it.
 *
 * The per-frame step is stored in g_battle_camera_real_coord_delta, the vector
 * the camera-position move also uses. No retail code calls it. */
void battle_camera_set_offset_screen_coord_delta_from_value(const s32* destination_value, s32 frame_count) {
    s32 animation_frames;
    s32 current_x;
    s32 current_z;
    s32 current_y;
    s32 delta_x;

    animation_frames = (frame_count & 0xffff) / g_main_system_vsync_interval;
    current_x = g_battle_offset_screen_coords.vx;
    current_z = g_battle_offset_screen_coords.vy;
    current_y = g_battle_offset_screen_coords.vz;
    g_battle_camera_offset_screen_coords_fixed.vx = current_x << 12;
    g_battle_camera_offset_screen_coords_fixed.vy = current_z << 12;
    g_battle_camera_offset_screen_coords_fixed.vz = current_y << 12;
    delta_x = *destination_value - current_x;
    g_battle_camera_offset_screen_coord_countdown = animation_frames;
    animation_frames = g_battle_camera_offset_screen_coord_countdown;
    g_battle_camera_real_coord_delta.vx = (delta_x << 12) / animation_frames;
    g_battle_camera_real_coord_delta.vy = ((*destination_value - current_z) << 12) / animation_frames;
    g_battle_camera_real_coord_delta.vz = ((*destination_value - current_y) << 12) / animation_frames;
}
