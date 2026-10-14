#include "fft/battle.h"
#include "psx/types.h"

/* View of the argument: the only caller passes the acting unit's
 * battle_unit_misc_data_t, whose SVECTOR at 0x40 is `screen` (vx = x,
 * vy = height, vz = map depth). */
typedef struct battle_camera_step_target {
    u8 _unused_00[0x40];
    SVECTOR coords; /* 0x40 */
} battle_camera_step_target_t;

/* Moves the camera position by this frame's step vector (g_battle_camera_step_vector)
 * toward the unit's position without passing it.
 *
 * Each component of g_battle_camera_current_real_coords (20.12) advances only
 * while it is still short of the target in the step's direction, and is set to
 * the target exactly when the step carries it past. Called for the acting unit
 * while it moves, chooses its wait direction or starts an attack animation. */
void battle_camera_step_real_coords_toward_target(battle_camera_step_target_t* target) {
    if (g_battle_camera_step_vector.vx < 0) {
        if (g_battle_camera_current_real_coords.vx / ONE > target->coords.vx) {
            g_battle_camera_current_real_coords.vx += g_battle_camera_step_vector.vx;
            if (g_battle_camera_current_real_coords.vx / ONE < target->coords.vx) {
                g_battle_camera_current_real_coords.vx = target->coords.vx << 12;
            }
        }
    } else {
        if (g_battle_camera_current_real_coords.vx / ONE < target->coords.vx) {
            g_battle_camera_current_real_coords.vx += g_battle_camera_step_vector.vx;
            if (g_battle_camera_current_real_coords.vx / ONE > target->coords.vx) {
                g_battle_camera_current_real_coords.vx = target->coords.vx << 12;
            }
        }
    }
    if (g_battle_camera_step_vector.vy < 0) {
        if (g_battle_camera_current_real_coords.vy / ONE > target->coords.vy) {
            g_battle_camera_current_real_coords.vy += g_battle_camera_step_vector.vy;
            if (g_battle_camera_current_real_coords.vy / ONE < target->coords.vy) {
                g_battle_camera_current_real_coords.vy = target->coords.vy << 12;
            }
        }
    } else {
        if (g_battle_camera_current_real_coords.vy / ONE < target->coords.vy) {
            g_battle_camera_current_real_coords.vy += g_battle_camera_step_vector.vy;
            if (g_battle_camera_current_real_coords.vy / ONE > target->coords.vy) {
                g_battle_camera_current_real_coords.vy = target->coords.vy << 12;
            }
        }
    }
    if (g_battle_camera_step_vector.vz < 0) {
        if (g_battle_camera_current_real_coords.vz / ONE > target->coords.vz) {
            g_battle_camera_current_real_coords.vz += g_battle_camera_step_vector.vz;
            if (g_battle_camera_current_real_coords.vz / ONE < target->coords.vz) {
                g_battle_camera_current_real_coords.vz = target->coords.vz << 12;
            }
        }
    } else {
        if (g_battle_camera_current_real_coords.vz / ONE < target->coords.vz) {
            /* Retail quirk: only this positive map-depth step doubles the component. */
            g_battle_camera_current_real_coords.vz += g_battle_camera_step_vector.vz * 2;
            if (g_battle_camera_current_real_coords.vz / ONE > target->coords.vz) {
                g_battle_camera_current_real_coords.vz = target->coords.vz << 12;
            }
        }
    }
}
