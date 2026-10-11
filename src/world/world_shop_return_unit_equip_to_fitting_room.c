#include "fft/world.h"

/* Clear one equipment slot of a formation unit. An item the unit owns is kept aside in
 * g_world_shop_fitting_room_items (it comes back if the fitting is undone); a tried-on shop item
 * (WORLD_EQUIPMENT_SHOP_ITEM) is taken off the fitting room's cost instead. */
void world_shop_return_unit_equip_to_fitting_room(s16 unit_index, s16 equipment_index) {
    s32 equipped_item = g_world_formation_unit_pointers[unit_index]->equipment[equipment_index];

    if (equipped_item != ITEM_ID_NOTHING) {
        if ((equipped_item >> 15) == 0)
            g_world_shop_fitting_room_items[unit_index][equipment_index] = equipped_item;
        else
            world_shop_add_fitting_room_cost(-world_item_get_price(equipped_item & WORLD_ITEM_ID_MASK));
        g_world_formation_unit_pointers[unit_index]->equipment[equipment_index] = ITEM_ID_NOTHING;
    }
}
