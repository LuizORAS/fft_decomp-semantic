#include "fft/world.h"
#include "psx/types.h"

/* Number for the unit cards' HP/MP/CT readout (menu script callbacks 0 and 1, set by
 * world_formation_init_selection_cursor): the unit's HP or MP, or a row of dashes for CT, which a unit
 * has only in battle. `value` is unset for any other mode; the level/EXP and Brave/Faith readouts use
 * callbacks 4-7. */
s32 world_formation_get_unit_stat_by_mode(s32 unit_index) {
    s32 value;

    if (g_world_formation_stat_display_mode == MENU_UNIT_READOUT_HP) {
        value = g_world_formation_unit_pointers[unit_index]->status.gauges[BATTLE_MENU_UNIT_STATUS_GAUGE_HP].value;
    } else if (g_world_formation_stat_display_mode == MENU_UNIT_READOUT_MP) {
        value = g_world_formation_unit_pointers[unit_index]->status.gauges[BATTLE_MENU_UNIT_STATUS_GAUGE_MP].value;
    } else if (g_world_formation_stat_display_mode == MENU_UNIT_READOUT_CT) {
        value = MENU_NUMBER_VALUE_DASHES | 100;
    }
    return value;
}
