#include "fft/battle_camera.h"
#include "fft/battle_effect.h"
#include "psx/types.h"

/* Starts the child timeline's camera moves for this frame: each of the
 * rotation, position and zoom tracks that is idle takes its next keyframe
 * from the child camera table. */
void battle_camera_start_child_keyframe_moves(s16 frame) {
    SVECTOR rotation;
    VECTOR vector;
    s32 index;
    s32 keyframe;

    if (g_battle_effect_camera_rotation_mode == BATTLE_EFFECT_CAMERA_MODE_IDLE
        && battle_effect_find_next_keyframe(frame, BATTLE_EFFECT_CAMERA_TRACK_ROTATION, &index, &keyframe) != 0) {
        battle_effect_copy_svector(&g_battle_effect_timeline->rotation[index], &rotation);
        battle_camera_execute_angle_command(
            g_battle_effect_timeline->command[index], keyframe / 0x10000, (keyframe & 0xFFFF) - frame, &rotation);
    }
    if (g_battle_effect_camera_position_mode == BATTLE_EFFECT_CAMERA_MODE_IDLE
        && battle_effect_find_next_keyframe(frame, BATTLE_EFFECT_CAMERA_TRACK_POSITION, &index, &keyframe) != 0) {
        battle_effect_convert_svector_to_vector(&g_battle_effect_timeline->position[index], &vector);
        battle_camera_execute_position_command(
            g_battle_effect_timeline->command[index], keyframe / 0x10000, (keyframe & 0xFFFF) - frame, &vector);
    }
    if (g_battle_effect_camera_zoom_mode == BATTLE_EFFECT_CAMERA_MODE_IDLE
        && battle_effect_find_next_keyframe(frame, BATTLE_EFFECT_CAMERA_TRACK_ZOOM, &index, &keyframe) != 0) {
        battle_effect_convert_svector_to_vector(&g_battle_effect_timeline->zoom[index], &vector);
        battle_camera_execute_zoom_command(
            g_battle_effect_timeline->command[index], keyframe / 0x10000, (keyframe & 0xFFFF) - frame, &vector);
    }
}
