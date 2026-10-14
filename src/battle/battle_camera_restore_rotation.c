#include "fft/battle.h"

void battle_camera_restore_rotation(void) {
    g_battle_camera_angles.vx = g_battle_camera_rotation_backup.vx;
    g_battle_camera_angles.vy = g_battle_camera_rotation_backup.vy;
    g_battle_camera_angles.vz = g_battle_camera_rotation_backup.vz;
}
