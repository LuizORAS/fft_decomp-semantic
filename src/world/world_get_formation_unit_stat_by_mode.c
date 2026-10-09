#include "fft/world.h"
#include "psx/types.h"

/* Pick the formation unit's stat by the current stat-select mode; `value` is unset for any other mode. */
s32 world_get_formation_unit_stat_by_mode(s32 unit_index) {
    s32 value;

    if (g_world_formation_stat_display_mode == WORLD_FORMATION_READOUT_HP) {
        value = g_world_formation_unit_pointers[unit_index]->hp;
    } else if (g_world_formation_stat_display_mode == WORLD_FORMATION_READOUT_MP) {
        value = g_world_formation_unit_pointers[unit_index]->mp;
    } else if (g_world_formation_stat_display_mode == WORLD_FORMATION_READOUT_CT) {
        value = MENU_NUMBER_VALUE_DASHES | 100;
    }
    return value;
}
