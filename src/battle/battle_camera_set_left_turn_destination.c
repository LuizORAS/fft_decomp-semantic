#include "fft/battle.h"
#include "psx/types.h"

/* Set the destination of a left turn (falling yaw) and record the yaw it leaves in
 * g_battle_camera_previous_yaw, which nothing reads. A destination on a map axis (a multiple of 0x400) is
 * moved 0x200 back, so the camera always comes to rest on a diagonal. battle_camera_update_rotation also
 * continues a held right turn through it: that destination is already a diagonal, where the two setters
 * agree. */
void battle_camera_set_left_turn_destination(s32 target_angle) {
    g_battle_camera_previous_yaw = g_battle_camera_angles.vy & 0xfe00;
    g_battle_camera_destination_yaw = target_angle;
    if ((target_angle & 0x3ff) == 0) {
        g_battle_camera_destination_yaw = target_angle + 0x200;
    }
}
