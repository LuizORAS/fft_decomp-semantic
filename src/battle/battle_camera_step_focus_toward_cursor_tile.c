#include "fft/battle.h"

/* Move one 12.4 fixed-point camera component toward its bound by its per-frame
 * step, without overshooting. The bound is compared against the component's
 * whole-unit part, so the clamp lands exactly on bound << 12. The test is
 * written once per sign of the step because the overshoot comparison reverses
 * with it. The body is a bare if/else: wrapped in do/while (0) it becomes a
 * loop to the compiler, which doubles its reference counts and changes the
 * register allocation. */
#define BATTLE_CAMERA_STEP_TOWARD(component, step, limit)                                                              \
    if (step < 0) {                                                                                                    \
        s32* position = component;                                                                                     \
        s32 whole = *position / 4096;                                                                                  \
        s32 bound = limit;                                                                                             \
        if (whole > bound) {                                                                                           \
            *position += step;                                                                                         \
            if (*position / 4096 < bound) {                                                                            \
                *position = bound << 12;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    } else {                                                                                                           \
        s32* position = component;                                                                                     \
        s32 whole = *position / 4096;                                                                                  \
        s32 bound = limit;                                                                                             \
        if (whole < bound) {                                                                                           \
            *position += step;                                                                                         \
            if (bound < *position / 4096) {                                                                            \
                *position = bound << 12;                                                                               \
            }                                                                                                          \
        }                                                                                                              \
    }

/*
 * Advance the camera focus toward the cursor tile by one frame's step.
 *
 * The horizontal bounds are the centre of the cursor tile (28 units per tile,
 * half a tile of bias); the caller supplies the height bound directly.
 */
void battle_camera_step_focus_toward_cursor_tile(s16 height) {
    BATTLE_CAMERA_STEP_TOWARD(
        &g_battle_camera_current_real_coords.vx, g_battle_camera_step_vector.vx, g_battle_cursor_x * 28 + 14);
    BATTLE_CAMERA_STEP_TOWARD(&g_battle_camera_current_real_coords.vy, g_battle_camera_step_vector.vy, height);
    BATTLE_CAMERA_STEP_TOWARD(
        &g_battle_camera_current_real_coords.vz, g_battle_camera_step_vector.vz, g_battle_cursor_y * 28 + 14);
}
