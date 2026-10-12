#include "fft/battle_action.h"
#include "fft/battle_camera.h"

/* Moves the camera position by this frame's step vector (g_battle_current_vector)
 * toward the unit's position without passing it.
 *
 * Each component of g_battle_camera_current_real_coords (20.12) advances only
 * while it is still short of the target in the step's direction, and is set to
 * the target exactly when the step carries it past. Called for the acting unit
 * while it moves, chooses its wait direction or starts an attack animation. */
void battle_camera_step_real_coords_toward_target(battle_unit_misc_data_t* target) {
    if (g_battle_current_vector.vx < 0) {
        if (g_battle_camera_current_real_coords.vx / ONE > target->screen.vx) {
            g_battle_camera_current_real_coords.vx += g_battle_current_vector.vx;
            if (g_battle_camera_current_real_coords.vx / ONE < target->screen.vx) {
                g_battle_camera_current_real_coords.vx = target->screen.vx << 12;
            }
        }
    } else {
        if (g_battle_camera_current_real_coords.vx / ONE < target->screen.vx) {
            g_battle_camera_current_real_coords.vx += g_battle_current_vector.vx;
            if (g_battle_camera_current_real_coords.vx / ONE > target->screen.vx) {
                g_battle_camera_current_real_coords.vx = target->screen.vx << 12;
            }
        }
    }
    if (g_battle_current_vector.vy < 0) {
        if (g_battle_camera_current_real_coords.vy / ONE > target->screen.vy) {
            g_battle_camera_current_real_coords.vy += g_battle_current_vector.vy;
            if (g_battle_camera_current_real_coords.vy / ONE < target->screen.vy) {
                g_battle_camera_current_real_coords.vy = target->screen.vy << 12;
            }
        }
    } else {
        if (g_battle_camera_current_real_coords.vy / ONE < target->screen.vy) {
            g_battle_camera_current_real_coords.vy += g_battle_current_vector.vy;
            if (g_battle_camera_current_real_coords.vy / ONE > target->screen.vy) {
                g_battle_camera_current_real_coords.vy = target->screen.vy << 12;
            }
        }
    }
    if (g_battle_current_vector.vz < 0) {
        if (g_battle_camera_current_real_coords.vz / ONE > target->screen.vz) {
            g_battle_camera_current_real_coords.vz += g_battle_current_vector.vz;
            if (g_battle_camera_current_real_coords.vz / ONE < target->screen.vz) {
                g_battle_camera_current_real_coords.vz = target->screen.vz << 12;
            }
        }
    } else {
        if (g_battle_camera_current_real_coords.vz / ONE < target->screen.vz) {
            /* Retail quirk: only this positive map-depth step doubles the component. */
            g_battle_camera_current_real_coords.vz += g_battle_current_vector.vz * 2;
            if (g_battle_camera_current_real_coords.vz / ONE > target->screen.vz) {
                g_battle_camera_current_real_coords.vz = target->screen.vz << 12;
            }
        }
    }
}
