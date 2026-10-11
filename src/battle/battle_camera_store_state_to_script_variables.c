#include "fft/battle.h"
#include "psx/types.h"

/* Publish the live camera rotation, zoom, and position to the script variable
 * block.
 *
 * The three zoom components all land in the same 32-bit script variable; that is what the
 * target does. */
void battle_camera_store_state_to_script_variables(void) {
    SVECTOR* rotation;
    VECTOR* zoom;
    VECTOR* coords;

    rotation = battle_camera_get_rotation();
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_TILT] = rotation->vx;
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_YAW] = rotation->vy;
    g_battle_camera_published_rotation_ptr = rotation;
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_ROLL] = rotation->vz;

    zoom = battle_camera_get_zoom();
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM] = zoom->vx;
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM] = zoom->vy;
    g_battle_camera_published_coords_ptr = zoom;
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_ZOOM] = zoom->vz;

    coords = battle_camera_get_current_real_coords();
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_X] = coords->vx;
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_Z] = coords->vy - (g_battle_camera_shake_z_offset << 12);
    g_battle_camera_published_coords_ptr = coords;
    g_battle_camera_shake_z_offset = 0;
    g_battle_script_variables[EVENT_SCRIPT_VAR_CAMERA_Y] = coords->vz;
}
