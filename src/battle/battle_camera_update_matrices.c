#include "fft/battle_camera.h"
#include "fft/battle_map.h"

/* Rebuilds the camera matrix from the angles (`rotation`, an SVECTOR), the
 * screen offset and the zoom.
 *
 * The matrix is the rotation scaled by the zoom, with a translation that puts
 * g_battle_camera_integer_coords (the negated camera position) at the screen
 * offset. It is stored in *camera_matrix, g_battle_map_camera_matrix and map mesh
 * part 0; the angles and zoom are kept in g_battle_map_camera_rotation and
 * g_battle_map_camera_zoom, and the light rotation matrix is rebuilt. The GTE is
 * left with the scaled rotation and the bare screen offset as its translation. */
void battle_camera_update_matrices(
    MATRIX* camera_matrix, SVECTOR* rotation, VECTOR* screen_offset, VECTOR* camera_zoom) {
    long result;

    g_battle_map_camera_zoom = *camera_zoom;
    memcpy(&g_battle_map_camera_rotation, rotation, 8);

    RotMatrix(rotation, camera_matrix);
    TransMatrix(camera_matrix, screen_offset);
    ScaleMatrix(camera_matrix, &g_battle_map_camera_zoom);
    RotMatrix(rotation, &g_battle_map_camera_matrix);
    SetRotMatrix(camera_matrix);
    SetTransMatrix(camera_matrix);
    RotTrans(&g_battle_camera_integer_coords, (VECTOR*)g_battle_map_camera_matrix.t, &result);
    ScaleMatrix(&g_battle_map_camera_matrix, &g_battle_map_camera_zoom);

    *camera_matrix = g_battle_map_camera_matrix;
    g_battle_map_mesh_parts[0].matrix = g_battle_map_camera_matrix;
    RotMatrix(&g_battle_map_light_rotation, &g_battle_map_light_rotation_matrix);
}
