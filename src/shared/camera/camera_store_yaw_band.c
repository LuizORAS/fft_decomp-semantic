#include "fft/battle_camera.h"
#include "psx/types.h"

/* Store in g_camera_yaw_band the ONE-wide (0x1000) band of [-0x4000, 0x4000) that
 * contains value; outside that range it is left unchanged. No code reads the result. */
void camera_store_yaw_band(s32 value) {
    s32 lower_bound;

    for (lower_bound = -0x4000; lower_bound < 0x3FF8; lower_bound += ONE) {
        if (value < lower_bound + ONE && value >= lower_bound) {
            g_camera_yaw_band = lower_bound;
        }
    }
}
