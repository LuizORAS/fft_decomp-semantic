#include "fft/battle.h"

/* Same as battle_camera_get_tile_sort_point_unbiased on a unit's position
 * (battle_unit_get_screen_data_ptr_by_misc_id). An absent unit passes address 0 on. No retail code calls it. */
void battle_camera_get_unit_sort_point_unbiased(u32 misc_id, s16* coords) {
    battle_camera_get_tile_sort_point_unbiased(
        (const s16*)battle_unit_get_screen_data_ptr_by_misc_id(misc_id & 0xffff), coords);
}
