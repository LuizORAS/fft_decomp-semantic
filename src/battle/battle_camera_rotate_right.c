#include "fft/battle.h"
#include "fft/battle_camera.h"
#include "psx/types.h"

/* Set the destination of a right turn (rising yaw) and record the yaw it
 * leaves. A destination on a map axis (a multiple of 0x400) is moved 0x200
 * back, so the camera always comes to rest on a diagonal. */
void battle_camera_rotate_right(s32 target_angle) {
    g_battle_previous_camera_yaw = g_battle_camera_render_state.vy & 0xfe00;
    g_battle_destination_camera_yaw = target_angle;
    if ((target_angle & 0x3ff) == 0) {
        g_battle_destination_camera_yaw = target_angle - 0x200;
    }
}
