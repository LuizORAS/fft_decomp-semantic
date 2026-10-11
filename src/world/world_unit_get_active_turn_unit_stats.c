#include "fft/battle.h"
#include "fft/world.h"

/* The battle stats of WORLD's active turn unit (g_world_active_turn_unit), as
 * battle_unit_get_active_turn_unit_stats does in BATTLE. */
battle_stats_t* world_unit_get_active_turn_unit_stats(void) {
    return battle_unit_get_stats_from_battle_id(g_world_active_turn_unit.battle_id);
}
