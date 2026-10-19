#include "fft/memory.h"
#include "fft/world.h"

/* WORLD twin of battle_menu_set_active_turn_panels. */
void world_menu_set_active_turn_panels(const battle_menu_unit_status_record_t* status,
    const world_menu_unit_status_identity_t* identity, const world_item_stat_detail_t* detail) {
    mem_copy_bytes_nonempty(&g_menu_selected_unit_status, status, sizeof(g_menu_selected_unit_status));
    mem_copy_bytes_nonempty(&g_menu_selected_unit, identity, sizeof(g_menu_selected_unit));
    mem_copy_bytes_nonempty(&g_menu_billboard_data, detail, sizeof(g_menu_billboard_data));
}
