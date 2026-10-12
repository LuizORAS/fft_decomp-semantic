#include "fft/world.h"
#include "psx/types.h"

/* Change the unit's job and drop what it can no longer use (world_formation_remove_invalid_unit_loadout).
 *
 * A Mime removes all equipment. With `update_equipment` (the job wheel passes it when the Max Equip
 * at Job Change option is on) the unit's items go back to the inventory, it takes its best fit
 * (world_unit_build_best_fit_equipment) and the banner shows "Equip to the maximum". The preview
 * record (WORLD_FORMATION_PREVIEW_RECORD) never exchanges inventory items. */
s32 world_formation_change_unit_job(s16 unit_id, s16 job_id, s32 update_equipment) {
    s16 equipment[5];
    s32 i;
    g_world_formation_unit_pointers[unit_id]->job_id = job_id;
    if (job_id == JOB_ID_MIME) {
        for (i = 0; i < 5; i++) {
            if (unit_id != WORLD_FORMATION_PREVIEW_RECORD) {
                world_item_adjust_inventory_count(g_world_formation_unit_pointers[unit_id]->equipment[i], 1);
            }
            g_world_formation_unit_pointers[unit_id]->equipment[i] = ITEM_ID_NOTHING;
        }
    }
    world_formation_save_records_to_party_data();
    world_formation_remove_invalid_unit_loadout(unit_id);
    if (update_equipment != 0) {
        if (unit_id != WORLD_FORMATION_PREVIEW_RECORD) {
            if (g_world_formation_unit_pointers[unit_id]->job_id != JOB_ID_MIME) {
                world_unit_build_best_fit_equipment(unit_id, equipment);
                for (i = 0; i < 5; i++) {
                    world_item_adjust_inventory_count(g_world_formation_unit_pointers[unit_id]->equipment[i], 1);
                }
                for (i = 0; i < 5; i++) {
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
