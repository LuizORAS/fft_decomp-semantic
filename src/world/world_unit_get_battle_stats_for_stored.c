#include "fft/battle_unit.h"
#include "fft/world.h"

unit_stats_t* world_unit_get_battle_stats_for_stored(void) {
    return battle_unit_get_stats_from_battle_id(g_world_active_unit_data.battle_id);
}
