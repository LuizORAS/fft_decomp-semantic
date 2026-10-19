#include "fft/world.h"

/* Install an item into a formation unit's equipment slot; returns the refusal of
 * world_formation_can_equip_item_in_slot (below 0) or 1.
 *
 * The slot's old item goes back to the inventory, and in a hand slot an item that does not pair with
 * the other hand (status 0) also sends that hand's item back. The roster is saved afterwards. The
 * preview record (WORLD_FORMATION_PREVIEW_RECORD) is updated without touching the inventory or the
 * roster.
 *
 * `item` is a separate u16 mirror of the masked argument: one s32 variable
 * reused for both the call argument and the stored value is copy-propagated and
 * swaps which of the two ends up in the callee-saved register. */
s32 world_formation_equip_item_to_unit_slot(s16 unit, s16 slot, s16 item_id) {
    s32 status;
    s32 other;
    u16 item;

    item_id &= WORLD_ITEM_ID_MASK;
    item = item_id;
    status = world_formation_can_equip_item_in_slot(unit, slot, item_id);
    if (status < 0) {
        return status;
    }
    if (unit != WORLD_FORMATION_PREVIEW_RECORD) {
        world_item_adjust_inventory_count(g_world_formation_unit_pointers[unit]->equipment[slot], 1);
    }
    if (slot < 2 && status != 1) {
        other = slot == 0;
        if (unit != WORLD_FORMATION_PREVIEW_RECORD) {
            world_item_adjust_inventory_count(g_world_formation_unit_pointers[unit]->equipment[other], 1);
        }
        g_world_formation_unit_pointers[unit]->equipment[other] = ITEM_ID_NOTHING;
    }
    g_world_formation_unit_pointers[unit]->equipment[slot] = item;
    if (unit != WORLD_FORMATION_PREVIEW_RECORD) {
        world_formation_save_records_to_party_data();
    }
    return 1;
}
