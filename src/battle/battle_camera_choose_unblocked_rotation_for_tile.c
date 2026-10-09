#include "fft/battle.h"
#include "psx/libgte.h"
#include "psx/types.h"

/* Copies the camera rotation, snapped to the nearest yaw quadrant whose view
 * of the tile is not blocked. The camera angles are read as an SVECTOR: the
 * quadrant is a signed short division, which unsigned halfwords do not emit. */
void battle_camera_choose_unblocked_rotation_for_tile(
    const battle_screen_coords_t* tile_coords, battle_camera_rotation_t* rotation_out) {
    map_tile_t* tile;
    SVECTOR* rotation;
    s32 mask;
    s32 quadrant;
    s32 yaw;

    tile = battle_map_get_tile_data_pointer(tile_coords->x, tile_coords->y, tile_coords->z);
    mask = tile->camera_block_masks & MAP_TILE_CAMERA_BLOCK_DIRECTION_MASK;
    rotation = (SVECTOR*)battle_camera_get_rotation();
    rotation_out->x = rotation->vx;
    rotation_out->z = rotation->vz;
    quadrant = (rotation->vy & 0xfff) / 0x400;
    yaw = rotation->vy & 0xe00;
    if ((g_battle_camera_quadrant_block_directions[quadrant] & mask) == 0
        || mask == MAP_TILE_CAMERA_BLOCK_DIRECTION_MASK) {
        rotation_out->y = yaw;
        return;
    }
    if ((g_battle_camera_quadrant_block_directions[(quadrant + 1) & 3] & mask) == 0) {
        rotation_out->y = yaw + 0x400;
    } else if ((g_battle_camera_quadrant_block_directions[(quadrant - 1) & 3] & mask) == 0) {
        rotation_out->y = yaw - 0x400;
    } else {
        rotation_out->y = (yaw + 0x800) & 0xfff;
    }
}
