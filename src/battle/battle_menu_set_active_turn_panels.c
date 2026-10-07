#include "fft/battle.h"
#include "psx/types.h"

/* Copy an active-turn banner, an active-unit record and a billboard into the records the turn panels
 * show (g_battle_menu_active_turn_banner, g_battle_active_turn_unit, g_battle_menu_billboard_data).
 * REQUIRE and DEBUGCHR show the unit being edited this way. */
void battle_menu_set_active_turn_panels(const u8* banner, const u8* unit, const u8* billboard) {
    battle_copy_bytes(&g_battle_menu_active_turn_banner, banner, sizeof(g_battle_menu_active_turn_banner));
    battle_copy_bytes(&g_battle_active_turn_unit, unit, sizeof(g_battle_active_turn_unit));
    battle_copy_bytes(g_battle_menu_billboard_data, billboard, sizeof(g_battle_menu_billboard_data));
}
