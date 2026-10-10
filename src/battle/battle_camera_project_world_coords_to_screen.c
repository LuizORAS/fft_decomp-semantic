#include "fft/battle.h"
#include "psx/types.h"

/* Projects a world position to screen x and y. Rebuilds the camera matrices first
 * and leaves them loaded in the GTE. */
void battle_camera_project_world_coords_to_screen(SVECTOR* world_coords, s16* screen_coords) {
    VECTOR transformed_coords;
    s32 result;

    battle_camera_update_matrices(
        &g_battle_camera_matrix, &g_battle_camera_render_state, &g_battle_offset_screen_coords, &g_battle_camera_zoom);
    SetRotMatrix(&g_battle_camera_matrix);
    SetTransMatrix(&g_battle_camera_matrix);
    RotTrans(world_coords, &transformed_coords, &result);
    screen_coords[0] = transformed_coords.vx;
    screen_coords[1] = transformed_coords.vy;
}
