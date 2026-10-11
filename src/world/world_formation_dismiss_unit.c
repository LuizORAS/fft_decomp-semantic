#include "fft/world.h"

/* Remove a formation unit from the roster, unequipping its five slots first
 * so the items return to the inventory. Does nothing when
 * world_formation_can_dismiss_unit refuses. */
void world_formation_dismiss_unit(s16 formation_idx) {
    s32 i;

    if (world_formation_can_dismiss_unit(formation_idx) != 0) {
        i = 0;
        do {
            world_formation_equip_item_to_unit_slot(formation_idx, i, ITEM_ID_NOTHING);
            i += 1;
        } while (i < WORLD_EQUIPMENT_SLOT_COUNT);
        main_party_remove_unit(g_world_formation_unit_pointers[formation_idx]->identity.roster_slot);
    }
}
