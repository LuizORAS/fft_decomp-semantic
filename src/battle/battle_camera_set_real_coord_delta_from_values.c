#include "fft/battle.h"

/* Starts a linear move of the camera position by the vector `values` (20.12) over
 * frame_count frames (divided by the animation speed). No retail code calls it. */
void battle_camera_set_real_coord_delta_from_values(const VECTOR* values, s32 frame_count) {
    s32 animation_frames;

    animation_frames = (frame_count & 0xffff) / g_main_system_vsync_interval;
    g_battle_camera_real_coord_countdown = animation_frames;
    animation_frames = g_battle_camera_real_coord_countdown;
    g_battle_camera_real_coord_delta.vx = values->vx / animation_frames;
    g_battle_camera_real_coord_delta.vy = values->vy / animation_frames;
    g_battle_camera_real_coord_delta.vz = values->vz / animation_frames;
}
