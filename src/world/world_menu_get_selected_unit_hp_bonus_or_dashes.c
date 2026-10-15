#include "fft/world.h"
#include "psx/types.h"

/* Menu script callback 4 of the item category menu: the previewed HP change, or dashes for none. */
s32 world_menu_get_selected_unit_hp_bonus_or_dashes(s32 unused_row) {
    s32 value = g_world_selected_unit_stat_summary.hp_bonus;
    if (value == 0) {
        return MENU_NUMBER_VALUE_DASHES;
    }
    return value;
}
