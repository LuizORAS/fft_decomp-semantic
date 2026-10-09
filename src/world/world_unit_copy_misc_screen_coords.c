#include "fft/battle.h"
#include "fft/world.h"

void world_unit_copy_misc_screen_coords(s32 misc_id, s16* out) {
    SVECTOR* screen_data = battle_unit_get_screen_data_ptr_by_misc_id(misc_id);

    if (screen_data != (SVECTOR*)-1) {
        out[0] = screen_data->vx;
        out[1] = screen_data->vy;
        out[2] = screen_data->vz;
    }
}
