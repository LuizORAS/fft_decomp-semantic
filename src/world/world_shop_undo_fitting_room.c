#include "fft/world.h"

/* Leave the fitting room without buying: every slot holding a tried-on item, or with an item
 * kept aside, gets the kept item back (nothing when none was kept), and the kept items are cleared. */
void world_shop_undo_fitting_room(void) {
    s32 i;
    s32 j;

    for (i = 0; i < g_world_formation_unit_count; i++) {
        for (j = 0; j < 5; j++) {
            if ((g_world_formation_unit_pointers[i]->equipment[j] >> 14) != 0
                || g_world_shop_fitting_room_items[i][j] != ITEM_ID_NOTHING) {
                g_world_formation_unit_pointers[i]->equipment[j] = g_world_shop_fitting_room_items[i][j];
            }
            g_world_shop_fitting_room_items[i][j] = ITEM_ID_NOTHING;
        }
    }
}
