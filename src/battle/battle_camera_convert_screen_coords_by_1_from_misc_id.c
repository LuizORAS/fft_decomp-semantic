#include "fft/battle_camera.h"

/* battle_camera_convert_screen_coords_modify_by_1 on a unit's position. An absent
 * unit passes address 0 on. No retail code calls it. */
void battle_camera_convert_screen_coords_by_1_from_misc_id(u32 misc_id, SVECTOR* coords) {
    battle_camera_convert_screen_coords_modify_by_1(
        battle_unit_get_screen_data_ptr_by_misc_id(misc_id & 0xffff), coords);
}
