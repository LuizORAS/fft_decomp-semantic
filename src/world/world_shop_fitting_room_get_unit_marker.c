#include "fft/world.h"
#include "psx/types.h"

/* Unit-grid marker callback of the fitting room's unit choice: no unit gets a marker (-1). */
s32 world_shop_fitting_room_get_unit_marker(s32 unused_unit_index) {
    return -1;
}
