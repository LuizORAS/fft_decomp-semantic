#include "fft/world.h"

/* Menu value callback 2 of the fitting room's cost window: the gil left after paying the
 * fitting room's cost (negative when it falls short). */
s32 world_shop_get_gil_minus_fitting_room_cost(s32 unused_row) {
    s32 gil;

    gil = world_shop_add_gil(0);
    return gil - world_shop_add_fitting_room_cost(0);
}
