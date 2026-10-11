#include "fft/battle.h"

/* The battle stats of the active turn unit (g_battle_active_turn_unit): the unit whose turn it is,
 * or the one the unit view shows. */
battle_stats_t* battle_unit_get_active_turn_unit_stats(void) {
    return battle_unit_get_stats_from_battle_id(g_battle_active_turn_unit.battle_id);
}
