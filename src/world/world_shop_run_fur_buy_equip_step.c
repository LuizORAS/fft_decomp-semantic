#include "fft/world.h"

/* Shop step FUR_BUY_EQUIP: world_shop_run_equip_candidate_step for the fur shop; leaving it
 * returns to the fur shop's buy list instead of the outfitter's. */
void world_shop_run_fur_buy_equip_step(void) {
    world_shop_run_equip_candidate_step();
    if (g_world_shop_menu_step == WORLD_SHOP_STEP_BUY_LIST)
        g_world_shop_menu_step = WORLD_SHOP_STEP_FUR_BUY_LIST;
}
