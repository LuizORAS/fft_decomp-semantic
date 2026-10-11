#include "fft/world.h"
#include "psx/types.h"

/* Menu script callback 5 of the item category menu: the previewed MP change, or dashes for none. */
s32 world_menu_get_selected_unit_mp_bonus_or_dashes(s32 unused_row) {
    s32 value = g_world_selected_unit_stat_summary.gauges[BATTLE_UNIT_STATUS_GAUGE_MP].bonus;
    if (value == 0) {
        return MENU_NUMBER_VALUE_DASHES;
    }
    return value;
}
