#include "fft/memory.h"
#include "fft/world.h"

/* Copy a unit's three panel blocks in: the 0x22-byte status billboard, the
 * 0xE-byte identity record and the 0x40-byte stat fields. */
void world_menu_copy_billboard_data_in(const menu_unit_status_record_t* status,
    const menu_unit_status_identity_t* identity, const menu_status_panel_slot_storage_t* detail) {
    mem_copy_bytes_nonempty(&g_menu_selected_unit_status, status, sizeof(g_menu_selected_unit_status));
    mem_copy_bytes_nonempty(&g_menu_selected_unit, identity, sizeof(g_menu_selected_unit));
    mem_copy_bytes_nonempty(&g_menu_selected_unit_fields, detail, sizeof(g_menu_selected_unit_fields));
}
