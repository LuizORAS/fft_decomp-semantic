#include "fft/world.h"
#include "psx/types.h"

/* Set ability slot `slot` of an unlocked formation unit and drop what it can no longer use; returns
 * 1, or -2 when the unit is locked. With `update_equipment` (an Equip, Martial Arts, Two Hands or
 * Two Swords support came or went with the Max Equip at Job Change option on,
 * world_ability_is_equip_support_change) the unit's items go back to the inventory, it takes its best
 * fit and the banner shows "Equip to the maximum". */
s32 world_formation_set_unit_ability_slot(s16 unit_id, s16 slot, s16 ability, s32 update_equipment) {
    s16 equipment[WORLD_EQUIPMENT_SLOT_COUNT];
    s32 i;
    if (world_formation_get_unit_lock(unit_id) == WORLD_FORMATION_UNIT_UNLOCKED) {
        world_unit_t* unit = g_world_formation_unit_pointers[unit_id];
        unit->abilities.slots[slot] = ability;
        world_formation_save_records_to_party_data();
        world_formation_remove_invalid_unit_loadout(unit_id);
        if (update_equipment != 0) {
            if (unit_id != WORLD_FORMATION_PREVIEW_RECORD) {
                if (g_world_formation_unit_pointers[unit_id]->identity.job_id != JOB_ID_MIME) {
                    world_unit_build_best_fit_equipment(unit_id, equipment);
                    for (i = 0; i < WORLD_EQUIPMENT_SLOT_COUNT; i++) {
                        world_item_adjust_inventory_count(g_world_formation_unit_pointers[unit_id]->equipment[i], 1);
                    }
                    for (i = 0; i < WORLD_EQUIPMENT_SLOT_COUNT; i++) {
                        g_world_formation_unit_pointers[unit_id]->equipment[i] = equipment[i];
                        world_item_adjust_inventory_count(equipment[i], -1);
                    }
                    world_formation_save_records_to_party_data();
                    world_menu_run_unit_status_banner_countdown(0xD811, 0x1E);
                }
            }
        }
        return 1;
    }
    return -2;
}
