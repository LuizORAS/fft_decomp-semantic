#include "fft/world.h"
#include "psx/types.h"

/* Fur shop version of the equip-candidate step: a return to the buy list
 * goes to the fur shop's buy list. */
void world_shop_run_fur_equip_purchase_step(void) {
    world_shop_run_equip_candidate_step();
    if (g_world_shop_menu_step == WORLD_SHOP_STEP_BUY_LIST)
        g_world_shop_menu_step = WORLD_SHOP_STEP_FUR_BUY_LIST;
}
