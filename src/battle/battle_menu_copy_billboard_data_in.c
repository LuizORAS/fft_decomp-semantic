#include "fft/battle.h"
#include "psx/types.h"

void battle_menu_copy_billboard_data_in(const void* banner, const void* unit, const void* billboard) {
    battle_copy_bytes(&g_battle_menu_active_turn_banner, banner, sizeof(g_battle_menu_active_turn_banner));
    battle_copy_bytes(&g_battle_active_turn_unit, unit, sizeof(g_battle_active_turn_unit));
    battle_copy_bytes(&g_battle_menu_billboard_data, billboard, sizeof(g_battle_menu_billboard_data));
}
