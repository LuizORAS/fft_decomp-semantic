#include "fft/world.h"

/* The on_change callback of the shop's quantity range (0x801950d8, the number range that
 * g_world_shop_quantity_menu_data binds: min 1, max g_world_shop_quantity_limit): nothing to do. */
void world_shop_menu_noop_callback(void) {
}
