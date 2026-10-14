#include "fft/unit.h"
#include "fft/world.h"

unit_stats_t* world_unit_get_active_stats(void) {
    return battle_unit_get_stats_from_battle_id(g_menu_selected_unit.battle_id);
}
