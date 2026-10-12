#include "fft/battle.h"
#include "psx/types.h"

/* Close the status menu: clear the status request and g_battle_status_menu_open, which switches the
 * other units' menus to their inspect variants (battle_menu_open_non_active_unit_idle_action_menu). */
void battle_menu_clear_status_menu_state(void) {
    g_battle_menu_status_requested = 0;
    g_battle_status_menu_open = 0;
}
