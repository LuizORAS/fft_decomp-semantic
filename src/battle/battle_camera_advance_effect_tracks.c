#include "fft/battle.h"
#include "psx/types.h"

/* Start the camera position, target and zoom moves toward the next keyframe
 * of each channel not locked by g_battle_effect_camera_rotation_mode/D0/D4, reading the keyframe from
 * the table battle_camera_find_active_keyframe reports (main, for-each-target or cleanup).
 * The three-table form of battle_camera_start_effect_keyframe_moves, also
 * known as advance_camera_tracks. */
void battle_camera_advance_effect_tracks(s16 frame) {
    SVECTOR position;
    VECTOR vector;
    s32 index;
    s32 keyframe;
    s32 k;

    if (g_battle_effect_camera_rotation_mode == BATTLE_EFFECT_CAMERA_MODE_IDLE
        && battle_camera_find_active_keyframe(frame, 1, &index, &keyframe) != 0) {
        k = index & 0xffff;
        switch (index / 0x10000) {
        case 0:
            battle_effect_copy_svector(g_battle_effect_timeline->main.position[k], &position);
            battle_camera_execute_angle_command(
                g_battle_effect_timeline->main.selector[k], keyframe / 0x10000, (keyframe & 0xffff) - frame, &position);
            break;
        case 1:
            battle_effect_copy_svector(g_battle_effect_timeline->position[k], &position);
            battle_camera_execute_angle_command(
                g_battle_effect_timeline->selector[k], keyframe / 0x10000, (keyframe & 0xffff) - frame, &position);
            break;
        case 2:
            battle_effect_copy_svector(g_battle_effect_timeline->cleanup.position[k], &position);
            battle_camera_execute_angle_command(g_battle_effect_timeline->cleanup.selector[k], keyframe / 0x10000,
                (keyframe & 0xffff) - frame, &position);
            break;
        }
    }
    if (g_battle_effect_camera_position_mode == BATTLE_EFFECT_CAMERA_MODE_IDLE
        && battle_camera_find_active_keyframe(frame, 2, &index, &keyframe) != 0) {
        k = index & 0xffff;
        switch (index / 0x10000) {
        case 0:
            battle_effect_convert_svector_to_vector(g_battle_effect_timeline->main.target[k], &vector);
            battle_camera_execute_position_command(
                g_battle_effect_timeline->main.selector[k], keyframe / 0x10000, (keyframe & 0xffff) - frame, &vector);
            break;
        case 1:
            battle_effect_convert_svector_to_vector(g_battle_effect_timeline->target[k], &vector);
            battle_camera_execute_position_command(
                g_battle_effect_timeline->selector[k], keyframe / 0x10000, (keyframe & 0xffff) - frame, &vector);
            break;
        case 2:
            battle_effect_convert_svector_to_vector(g_battle_effect_timeline->cleanup.target[k], &vector);
            battle_camera_execute_position_command(g_battle_effect_timeline->cleanup.selector[k], keyframe / 0x10000,
                (keyframe & 0xffff) - frame, &vector);
            break;
        }
    }
    if (g_battle_effect_camera_zoom_mode == BATTLE_EFFECT_CAMERA_MODE_IDLE
        && battle_camera_find_active_keyframe(frame, 4, &index, &keyframe) != 0) {
        k = index & 0xffff;
        switch (index / 0x10000) {
        case 0:
            battle_effect_convert_svector_to_vector(g_battle_effect_timeline->main.zoom[k], &vector);
            battle_camera_execute_zoom_command(
                g_battle_effect_timeline->main.selector[k], keyframe / 0x10000, (keyframe & 0xffff) - frame, &vector);
            break;
        case 1:
            battle_effect_convert_svector_to_vector(g_battle_effect_timeline->zoom[k], &vector);
            battle_camera_execute_zoom_command(
                g_battle_effect_timeline->selector[k], keyframe / 0x10000, (keyframe & 0xffff) - frame, &vector);
            break;
        case 2:
            battle_effect_convert_svector_to_vector(g_battle_effect_timeline->cleanup.zoom[k], &vector);
            battle_camera_execute_zoom_command(g_battle_effect_timeline->cleanup.selector[k], keyframe / 0x10000,
                (keyframe & 0xffff) - frame, &vector);
            break;
        }
    }
}
