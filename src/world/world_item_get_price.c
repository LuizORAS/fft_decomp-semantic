#include "fft/world.h"

/* An item's price in the item table. */
s32 world_item_get_price(s16 item_id) {
    return g_main_item_primary_data[item_id & WORLD_ITEM_ID_MASK].price;
}
