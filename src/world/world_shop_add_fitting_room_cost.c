#include "fft/world.h"
#include "psx/types.h"

/* Menu callback: add `delta` to the fitting room's running cost and return
 * the new total. */
s32 world_shop_add_fitting_room_cost(s32 delta) {
    return g_world_shop_fitting_room_cost = delta + g_world_shop_fitting_room_cost;
}
