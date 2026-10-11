#include "fft/world.h"

/* Take a tried-on item off one equipment slot of formation unit formation_index and put back
 * the item the fitting room kept for that slot. For a hand, the kept item returns only when the unit
 * can hold it there with the other hand's item, and when both hands are now empty both kept hand items
 * return. The caller refunds the price. */
void world_shop_take_off_tried_on_item(s16 formation_index, s16 equipment_index) {
    u16 reserved_item;
    s16 unit_index = formation_index;

    g_world_formation_unit_pointers[unit_index]->equipment[equipment_index] = ITEM_ID_NOTHING;
    reserved_item = g_world_shop_fitting_room_items[unit_index][equipment_index];
    if (equipment_index < 2) {
        u16 right_hand = g_world_formation_unit_pointers[unit_index]->equipment[0];
        u16 left_hand = g_world_formation_unit_pointers[unit_index]->equipment[1];

        if (right_hand == ITEM_ID_NOTHING && left_hand == ITEM_ID_NOTHING) {
            g_world_formation_unit_pointers[unit_index]->equipment[0] = g_world_shop_fitting_room_items[unit_index][0];
            g_world_formation_unit_pointers[unit_index]->equipment[1] = g_world_shop_fitting_room_items[unit_index][1];
            g_world_shop_fitting_room_items[unit_index][0] = ITEM_ID_NOTHING;
            g_world_shop_fitting_room_items[unit_index][1] = ITEM_ID_NOTHING;
            return;
        }
        if (reserved_item != ITEM_ID_NOTHING
            && world_formation_can_equip_item_in_slot(
                   g_world_formation_selected_unit_index, equipment_index, reserved_item)
                == 1) {
            g_world_formation_unit_pointers[unit_index]->equipment[equipment_index] = reserved_item;
            g_world_shop_fitting_room_items[unit_index][equipment_index] = ITEM_ID_NOTHING;
        }
    } else {
        g_world_formation_unit_pointers[unit_index]->equipment[equipment_index] = reserved_item;
        g_world_shop_fitting_room_items[unit_index][equipment_index] = ITEM_ID_NOTHING;
    }
}
