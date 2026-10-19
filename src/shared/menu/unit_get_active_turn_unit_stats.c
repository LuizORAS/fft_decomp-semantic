#include "fft/menu.h"
#include "fft/unit.h"

/* The battle stats of the active turn unit (g_menu_selected_unit): the unit whose turn it is,
 * or the one the unit view shows. WORLD compiles this text too (world_unit_get_selected_unit_stats). */
battle_stats_t* unit_get_active_turn_unit_stats(void) {
    return battle_unit_get_stats_from_battle_id(g_menu_selected_unit.battle_id);
}
