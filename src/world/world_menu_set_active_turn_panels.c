#include "fft/world.h"
#include "psx/types.h"

/* Copy a unit's three panel blocks in: the 0x22-byte status billboard, the
 * 0xE-byte identity record and the 0x40-byte stat fields. */
void world_menu_set_active_turn_panels(const void* source_a, const void* source_b, const void* source_c) {
    world_copy_bytes(&g_world_menu_active_turn_banner, source_a, sizeof(g_world_menu_active_turn_banner));
    world_copy_bytes(&g_world_active_turn_unit, source_b, sizeof(g_world_active_turn_unit));
    world_copy_bytes(&g_world_menu_billboard_data, source_c, sizeof(g_world_menu_billboard_data));
}
