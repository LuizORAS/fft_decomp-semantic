#include "fft/battle_camera.h"

/* Same as battle_camera_get_tile_sort_point_unbiased on a unit's position
 * (battle_unit_get_world_coords_by_runtime_id). An absent unit passes address 0 on. No retail code calls it. */
void battle_camera_get_unit_sort_point_unbiased(u32 runtime_id, SVECTOR* coords) {
    battle_camera_get_tile_sort_point_unbiased(battle_unit_get_world_coords_by_runtime_id(runtime_id & 0xffff), coords);
}
