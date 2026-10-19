#include "fft/battle_camera.h"
#include "fft/battle_script.h"
#include "fft/script.h"

/* Load camera rotation, zoom and position back out of the script variable
 * block; the inverse of camera_store_state_to_script_variables. */
void battle_camera_load_state_from_script_variables(void) {
    setVector(&g_battle_camera_script_rotation, g_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT],
        g_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW], g_script_variables[EVENT_SCRIPT_VAR_CAMERA_ROLL]);
    battle_camera_set_rotation(&g_battle_camera_script_rotation);

    setVector(&g_battle_camera_script_vector, g_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM],
        g_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM], g_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM]);
    battle_camera_set_zoom(&g_battle_camera_script_vector);

    g_battle_camera_script_vector.vx = g_script_variables[EVENT_SCRIPT_VAR_CAMERA_X];
    g_battle_camera_script_vector.vy
        = (g_battle_camera_shake_z_offset * ONE) + g_script_variables[EVENT_SCRIPT_VAR_CAMERA_Z];
    g_battle_camera_script_vector.vz = g_script_variables[EVENT_SCRIPT_VAR_CAMERA_Y];
    battle_camera_set_current_real_coords(&g_battle_camera_script_vector);
}
