#include "fft/battle_camera.h"
#include "fft/main.h"
#include "psx/libgpu.h"

/* Starts a linear change of the zoom scales by the vector `values` over
 * frame_count frames (divided by the animation speed). No retail code calls it. */
void battle_camera_set_zoom_delta_from_values(const VECTOR* values, s32 frame_count) {
    s32 animation_frames;

    animation_frames = (frame_count & 0xffff) / g_main_system_animation_speed;
    g_battle_camera_zoom_countdown = animation_frames;
    animation_frames = g_battle_camera_zoom_countdown;
    setVector(&g_battle_camera_zoom_delta, values->vx / animation_frames, values->vy / animation_frames,
        values->vz / animation_frames);
}
