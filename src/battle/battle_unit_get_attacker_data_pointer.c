#include "fft/battle_action.h"
#include "fft/battle_unit.h"

/* The battle stats of the unit whose turn it is (g_menu_selected_unit). */
unit_stats_t* battle_unit_get_attacker_data_pointer(void) {
    return battle_unit_get_stats_from_battle_id(g_menu_selected_unit.battle_id);
}
