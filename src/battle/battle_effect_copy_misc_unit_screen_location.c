#include "fft/battle_effect.h"

/* Effect-camera twin of battle_unit_copy_misc_screen_location (0x801330e4):
 * widens the unit's screen X/Z/Y 16-bit values into three 32-bit components
 * (used for effect camera targets). */
void battle_effect_copy_misc_unit_screen_location(u32 misc_id, VECTOR* destination) {
    SVECTOR* screen_data;

    screen_data = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);
    destination->vx = screen_data->vx;
    destination->vy = screen_data->vy;
    destination->vz = screen_data->vz;
}
