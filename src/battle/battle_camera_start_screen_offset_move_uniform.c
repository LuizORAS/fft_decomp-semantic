#include "fft/battle.h"
#include "fft/battle_camera.h"

/* Starts a linear move of all three screen-offset components to the one value
 * *destination_value over frame_count frames (divided by the animation speed).
 * battle_camera_update_offset_screen_coord_animation applies it.
 *
 * The per-frame step is stored in g_battle_camera_real_coord_delta, the vector
 * the camera-position move also uses. No retail code calls it. */
void battle_camera_start_screen_offset_move_uniform(const s32* destination_value, s32 frame_count) {
    s32 animation_frames;
    s32 current_x;
    s32 current_z;
    s32 current_y;
    s32 delta_x;

    animation_frames = (frame_count & 0xffff) / g_main_system_vsync_interval;
    current_x = g_battle_offset_screen_coords.vx;
    current_z = g_battle_offset_screen_coords.vy;
    current_y = g_battle_offset_screen_coords.vz;
    setVector(&g_battle_camera_offset_screen_coords_fixed, current_x * ONE, current_z * ONE, current_y * ONE);
    delta_x = *destination_value - current_x;
    g_battle_camera_offset_screen_coord_countdown = animation_frames;
    animation_frames = g_battle_camera_offset_screen_coord_countdown;
    setVector(&g_battle_camera_real_coord_delta, (delta_x * ONE) / animation_frames,
        ((*destination_value - current_z) * ONE) / animation_frames,
        ((*destination_value - current_y) * ONE) / animation_frames);
}
