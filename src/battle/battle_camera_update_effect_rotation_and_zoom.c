#include "fft/battle.h"
#include "psx/types.h"

/* Advance the effect camera rotation, position and zoom tracks one frame.
 *
 * Each track's mode word (g_battle_effect_camera_rotation_mode, _position_mode and _zoom_mode: bits 0x1e00 of the
 * timeline command) selects an interpolation between the start and target values over the track duration; the mode
 * clears once the counter reaches the duration. */
void battle_camera_update_effect_rotation_and_zoom(void) {
    VECTOR position;
    VECTOR delta;
    SVECTOR rotation;
    SVECTOR rotation_delta;

    switch (g_battle_effect_camera_rotation_mode) {
    case BATTLE_EFFECT_CAMERA_MODE_IDLE:
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SNAP:
        battle_camera_set_rotation(&g_battle_effect_camera_rotation_target);
        g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        g_battle_effect_camera_rotation_start.vx = g_battle_effect_camera_rotation_current.vx
            = g_battle_effect_camera_rotation_target.vx & 0xfff;
        g_battle_effect_camera_rotation_start.vy = g_battle_effect_camera_rotation_current.vy
            = g_battle_effect_camera_rotation_target.vy & 0xfff;
        g_battle_effect_camera_rotation_start.vz = g_battle_effect_camera_rotation_current.vz
            = g_battle_effect_camera_rotation_target.vz & 0xfff;
        g_battle_effect_camera_rotation_frame = g_battle_effect_camera_rotation_duration;
        break;
    case BATTLE_EFFECT_CAMERA_MODE_COSINE:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            battle_effect_interpolate_svector_cos(&g_battle_effect_camera_rotation_start,
                &g_battle_effect_camera_rotation_target, g_battle_effect_camera_rotation_duration,
                g_battle_effect_camera_rotation_frame, &g_battle_effect_camera_rotation_current);
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_current);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            battle_effect_mask_vector_low_12_bits(
                &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_start);
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_start);
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_COSINE_Q12:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            battle_effect_interpolate_svector_cos_q12(&g_battle_effect_camera_rotation_start,
                &g_battle_effect_camera_rotation_target, g_battle_effect_camera_rotation_duration,
                g_battle_effect_camera_rotation_frame, &g_battle_effect_camera_rotation_current);
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_current);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_LINEAR:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            battle_effect_interpolate_svector_linear(&g_battle_effect_camera_rotation_start,
                &g_battle_effect_camera_rotation_target, g_battle_effect_camera_rotation_duration,
                g_battle_effect_camera_rotation_frame, &g_battle_effect_camera_rotation_current);
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_current);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            battle_effect_mask_vector_low_12_bits(
                &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_start);
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_start);
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD_ACCELERATING:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            rotation.vx = rotation.vy = rotation.vz = 0;
            battle_effect_interpolate_svector_linear(&rotation, &g_battle_effect_camera_rotation_target,
                g_battle_effect_camera_rotation_duration, g_battle_effect_camera_rotation_frame, &rotation_delta);
            battle_effect_add_rotation_vectors_mod_q12(
                &g_battle_effect_camera_rotation_current, &rotation_delta, &g_battle_effect_camera_rotation_current);
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_current);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_target = g_battle_effect_camera_rotation_current;
            g_battle_effect_camera_rotation_start = g_battle_effect_camera_rotation_target;
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            battle_effect_add_rotation_vectors_mod_q12(&g_battle_effect_camera_rotation_current,
                &g_battle_effect_camera_rotation_target, &g_battle_effect_camera_rotation_current);
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_current);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_target = g_battle_effect_camera_rotation_current;
            g_battle_effect_camera_rotation_start = g_battle_effect_camera_rotation_target;
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD_DECELERATING:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            rotation.vx = rotation.vy = rotation.vz = 0;
            battle_effect_interpolate_svector_linear(&g_battle_effect_camera_rotation_target, &rotation,
                g_battle_effect_camera_rotation_duration, g_battle_effect_camera_rotation_frame, &rotation_delta);
            battle_effect_add_rotation_vectors_mod_q12(
                &g_battle_effect_camera_rotation_current, &rotation_delta, &g_battle_effect_camera_rotation_current);
            battle_camera_set_rotation(&g_battle_effect_camera_rotation_current);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_target = g_battle_effect_camera_rotation_current;
            g_battle_effect_camera_rotation_start = g_battle_effect_camera_rotation_target;
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE_GROWING:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            rotation.vx = rotation.vy = rotation.vz = 0;
            battle_effect_interpolate_svector_linear(&rotation, &g_battle_effect_camera_rotation_target,
                g_battle_effect_camera_rotation_duration, g_battle_effect_camera_rotation_frame, &rotation_delta);
            battle_effect_add_random_rotation_offsets(
                &g_battle_effect_camera_rotation_start, &rotation_delta, &rotation);
            battle_camera_set_rotation(&rotation);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            battle_effect_add_random_rotation_offsets(
                &g_battle_effect_camera_rotation_start, &g_battle_effect_camera_rotation_target, &rotation);
            battle_camera_set_rotation(&rotation);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE_DECAYING:
        if (g_battle_effect_camera_rotation_duration > g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_frame++;
            rotation.vx = rotation.vy = rotation.vz = 0;
            battle_effect_interpolate_svector_linear(&g_battle_effect_camera_rotation_target, &rotation,
                g_battle_effect_camera_rotation_duration, g_battle_effect_camera_rotation_frame, &rotation_delta);
            battle_effect_add_random_rotation_offsets(
                &g_battle_effect_camera_rotation_start, &rotation_delta, &rotation);
            battle_camera_set_rotation(&rotation);
        }
        if (g_battle_effect_camera_rotation_duration == g_battle_effect_camera_rotation_frame) {
            g_battle_effect_camera_rotation_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    }

    switch (g_battle_effect_camera_position_mode) {
    case BATTLE_EFFECT_CAMERA_MODE_IDLE:
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SNAP:
        battle_effect_shift_vector_left_12(&g_battle_effect_camera_position_target, &position);
        battle_camera_set_current_real_coords(&position);
        g_battle_effect_camera_position_current = g_battle_effect_camera_position_target;
        g_battle_effect_camera_position_start = g_battle_effect_camera_position_current;
        g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        g_battle_effect_camera_position_frame = g_battle_effect_camera_position_duration;
        break;
    case BATTLE_EFFECT_CAMERA_MODE_COSINE:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            battle_effect_interpolate_vector_q13_cos(&g_battle_effect_camera_position_start,
                &g_battle_effect_camera_position_target, g_battle_effect_camera_position_duration,
                g_battle_effect_camera_position_frame, &g_battle_effect_camera_position_current);
            battle_effect_shift_vector_left_12(&g_battle_effect_camera_position_current, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_start = g_battle_effect_camera_position_target;
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_COSINE_Q12:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            battle_effect_interpolate_vector_q13_cos_q12(&g_battle_effect_camera_position_start,
                &g_battle_effect_camera_position_target, g_battle_effect_camera_position_duration,
                g_battle_effect_camera_position_frame, &g_battle_effect_camera_position_current);
            battle_effect_shift_vector_left_12(&g_battle_effect_camera_position_current, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_LINEAR:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            battle_effect_interpolate_vector_q13_linear(&g_battle_effect_camera_position_start,
                &g_battle_effect_camera_position_target, g_battle_effect_camera_position_duration,
                g_battle_effect_camera_position_frame, &g_battle_effect_camera_position_current);
            battle_effect_shift_vector_left_12(&g_battle_effect_camera_position_current, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_start = g_battle_effect_camera_position_target;
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD_ACCELERATING:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&position, &g_battle_effect_camera_position_target,
                g_battle_effect_camera_position_duration, g_battle_effect_camera_position_frame, &delta);
            battle_effect_add_vector_and_store_q12(&delta, &g_battle_effect_camera_position_current, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_target = g_battle_effect_camera_position_current;
            g_battle_effect_camera_position_start = g_battle_effect_camera_position_target;
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            battle_effect_add_vector_and_store_q12(
                &g_battle_effect_camera_position_target, &g_battle_effect_camera_position_current, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_target = g_battle_effect_camera_position_current;
            g_battle_effect_camera_position_start = g_battle_effect_camera_position_target;
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD_DECELERATING:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&g_battle_effect_camera_position_target, &position,
                g_battle_effect_camera_position_duration, g_battle_effect_camera_position_frame, &delta);
            battle_effect_add_vector_and_store_q12(&delta, &g_battle_effect_camera_position_current, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_target = g_battle_effect_camera_position_current;
            g_battle_effect_camera_position_start = g_battle_effect_camera_position_target;
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE_GROWING:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&position, &g_battle_effect_camera_position_target,
                g_battle_effect_camera_position_duration, g_battle_effect_camera_position_frame, &delta);
            battle_effect_add_random_vector_offsets_and_store_q12(
                &g_battle_effect_camera_position_start, &delta, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            battle_effect_add_random_vector_offsets_and_store_q12(
                &g_battle_effect_camera_position_start, &g_battle_effect_camera_position_target, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE_DECAYING:
        if (g_battle_effect_camera_position_duration > g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&g_battle_effect_camera_position_target, &position,
                g_battle_effect_camera_position_duration, g_battle_effect_camera_position_frame, &delta);
            battle_effect_add_random_vector_offsets_and_store_q12(
                &g_battle_effect_camera_position_start, &delta, &position);
            battle_camera_set_current_real_coords(&position);
        }
        if (g_battle_effect_camera_position_duration == g_battle_effect_camera_position_frame) {
            g_battle_effect_camera_position_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    }

    switch (g_battle_effect_camera_zoom_mode) {
    case BATTLE_EFFECT_CAMERA_MODE_IDLE:
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SNAP:
        battle_camera_set_zoom(&g_battle_effect_camera_zoom_target);
        g_battle_effect_camera_zoom_current = g_battle_effect_camera_zoom_target;
        g_battle_effect_camera_zoom_start = g_battle_effect_camera_zoom_current;
        g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        g_battle_effect_camera_zoom_frame = g_battle_effect_camera_zoom_duration;
        break;
    case BATTLE_EFFECT_CAMERA_MODE_COSINE:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            battle_effect_interpolate_vector_q13_cos(&g_battle_effect_camera_zoom_start,
                &g_battle_effect_camera_zoom_target, g_battle_effect_camera_zoom_duration,
                g_battle_effect_camera_zoom_frame, &g_battle_effect_camera_zoom_current);
            battle_camera_set_zoom(&g_battle_effect_camera_zoom_current);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_start = g_battle_effect_camera_zoom_target;
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_COSINE_Q12:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            battle_effect_interpolate_vector_q13_cos_q12(&g_battle_effect_camera_zoom_start,
                &g_battle_effect_camera_zoom_target, g_battle_effect_camera_zoom_duration,
                g_battle_effect_camera_zoom_frame, &g_battle_effect_camera_zoom_current);
            battle_camera_set_zoom(&g_battle_effect_camera_zoom_current);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_LINEAR:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            battle_effect_interpolate_vector_q13_linear(&g_battle_effect_camera_zoom_start,
                &g_battle_effect_camera_zoom_target, g_battle_effect_camera_zoom_duration,
                g_battle_effect_camera_zoom_frame, &g_battle_effect_camera_zoom_current);
            battle_camera_set_zoom(&g_battle_effect_camera_zoom_current);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_start = g_battle_effect_camera_zoom_target;
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD_ACCELERATING:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&position, &g_battle_effect_camera_zoom_target,
                g_battle_effect_camera_zoom_duration, g_battle_effect_camera_zoom_frame, &delta);
            battle_effect_add_vectors(
                &delta, &g_battle_effect_camera_zoom_current, &g_battle_effect_camera_zoom_current);
            battle_camera_set_zoom(&g_battle_effect_camera_zoom_current);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_target = g_battle_effect_camera_zoom_current;
            g_battle_effect_camera_zoom_start = g_battle_effect_camera_zoom_target;
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            battle_effect_add_vectors(&g_battle_effect_camera_zoom_target, &g_battle_effect_camera_zoom_current,
                &g_battle_effect_camera_zoom_current);
            battle_camera_set_zoom(&g_battle_effect_camera_zoom_current);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_target = g_battle_effect_camera_zoom_current;
            g_battle_effect_camera_zoom_start = g_battle_effect_camera_zoom_target;
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_ADD_DECELERATING:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&g_battle_effect_camera_zoom_target, &position,
                g_battle_effect_camera_zoom_duration, g_battle_effect_camera_zoom_frame, &delta);
            battle_effect_add_vectors(
                &delta, &g_battle_effect_camera_zoom_current, &g_battle_effect_camera_zoom_current);
            battle_camera_set_zoom(&g_battle_effect_camera_zoom_current);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_target = g_battle_effect_camera_zoom_current;
            g_battle_effect_camera_zoom_start = g_battle_effect_camera_zoom_target;
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE_GROWING:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&position, &g_battle_effect_camera_zoom_target,
                g_battle_effect_camera_zoom_duration, g_battle_effect_camera_zoom_frame, &delta);
            battle_effect_add_random_vector_offsets(&g_battle_effect_camera_zoom_start, &delta, &position);
            battle_camera_set_zoom(&position);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            battle_effect_add_random_vector_offsets(
                &g_battle_effect_camera_zoom_start, &g_battle_effect_camera_zoom_target, &position);
            battle_camera_set_zoom(&position);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    case BATTLE_EFFECT_CAMERA_MODE_SHAKE_DECAYING:
        if (g_battle_effect_camera_zoom_duration > g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_frame++;
            position.vx = position.vy = position.vz = 0;
            battle_effect_interpolate_vector_q13_linear(&g_battle_effect_camera_zoom_target, &position,
                g_battle_effect_camera_zoom_duration, g_battle_effect_camera_zoom_frame, &delta);
            battle_effect_add_random_vector_offsets(&g_battle_effect_camera_zoom_start, &delta, &position);
            battle_camera_set_zoom(&position);
        }
        if (g_battle_effect_camera_zoom_duration == g_battle_effect_camera_zoom_frame) {
            g_battle_effect_camera_zoom_mode = BATTLE_EFFECT_CAMERA_MODE_IDLE;
        }
        break;
    }
}
