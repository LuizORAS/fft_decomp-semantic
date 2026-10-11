#include "fft/world.h"
#include "psx/types.h"

/* Add delta to the fitting room's cost (the prices of the tried-on shop items) and return
 * the new total; 0 reads it. Menu value callback 1 of the fitting room's cost window. */
s32 world_shop_add_fitting_room_cost(s32 delta) {
    return g_world_shop_fitting_room_cost = delta + g_world_shop_fitting_room_cost;
}
