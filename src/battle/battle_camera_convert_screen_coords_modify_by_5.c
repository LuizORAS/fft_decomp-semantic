#include "fft/battle.h"
#include "psx/types.h"

/* battle_camera_convert_screen_coords_by_rotation with offset 5: the depth point on a tile with
 * MAP_TILE_SURFACE_FLAG_DEPTH_BIAS. */
void battle_camera_convert_screen_coords_modify_by_5(const s16* screen, s16* destination) {
    battle_camera_convert_screen_coords_by_rotation(screen, destination, 5);
}
