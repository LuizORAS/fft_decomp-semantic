#include "fft/world.h"

/* Inventory quantity plus the copies equipped by the party. */
s32 world_item_count_owned_and_equipped(s16 item_id) {
    item_id &= WORLD_ITEM_ID_MASK;
    return world_item_adjust_inventory_count(item_id, 0) + world_count_item_equipped_by_party(item_id);
}
