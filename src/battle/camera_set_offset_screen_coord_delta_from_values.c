#include "fft/battle.h"

/* Starts a linear move of the screen offset by the vector `values` over
 * frame_count frames (divided by the animation speed).
 *
 * The per-frame step is stored in g_camera_real_coord_delta, the vector
 * the camera-position move also uses. No retail code calls it. */
void camera_set_offset_screen_coord_delta_from_values(const VECTOR* values, s32 frame_count) {
    s32 animation_frames;

    animation_frames = (frame_count & 0xffff) / g_main_system_animation_speed;
    setVector(&g_camera_offset_screen_coords_fixed, g_battle_offset_screen_coords.vx << 12,
        g_battle_offset_screen_coords.vy << 12, g_battle_offset_screen_coords.vz << 12);
    g_camera_offset_screen_coord_countdown = animation_frames;
    animation_frames = g_camera_offset_screen_coord_countdown;
    setVector(&g_camera_real_coord_delta, (values->vx << 12) / animation_frames, (values->vy << 12) / animation_frames,
        (values->vz << 12) / animation_frames);
}
