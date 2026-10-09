#include "fft/world.h"
#include "psx/types.h"

s32 world_menu_get_display_value_2(s32 unused_row) {
    s32 value = g_world_selected_unit_stat_summary.mp_bonus;
    if (value == 0) {
        return MENU_NUMBER_VALUE_DASHES;
    }
    return value;
}
