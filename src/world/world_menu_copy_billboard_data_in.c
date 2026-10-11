#include "fft/world.h"
#include "psx/types.h"

/* Copy a unit's three panel blocks in: the 0x22-byte status billboard, the
 * 0xE-byte identity record and the 0x40-byte stat fields. */
void world_menu_copy_billboard_data_in(const menu_unit_status_record_t* status,
    const menu_unit_status_identity_t* identity, const menu_item_stat_detail_t* detail) {
    world_script_copy_bytes(
        &g_world_unit_selected_status_billboard, status, sizeof(g_world_unit_selected_status_billboard));
    world_script_copy_bytes(&g_world_active_unit_data, identity, sizeof(g_world_active_unit_data));
    world_script_copy_bytes(&g_world_unit_editor_fields, detail, sizeof(g_world_unit_editor_fields));
}
