#include "fft/world.h"

void world_shop_buy_from_fitting_room(void) {
    s32 i;
    s32 j;

    for (i = 0; i < g_world_formation_unit_count; i++) {
        for (j = 0; j < 5; j++) {
            s32 equipped_item = g_world_formation_unit_pointers[i]->equipment[j];
            if ((equipped_item >> 14) != 0) {
                g_world_formation_unit_pointers[i]->equipment[j] &= WORLD_ITEM_ID_MASK;
            }
            world_item_adjust_inventory_count(g_world_shop_fitting_room_items[i][j], 1);
            g_world_shop_fitting_room_items[i][j] = ITEM_ID_NOTHING;
        }
    }
}
