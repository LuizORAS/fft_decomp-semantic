#include "fft/world.h"
#include "psx/types.h"

s32 world_menu_get_value_1(s32 unused_row) {
    return g_world_selected_unit_stat_summary.hp_bonus;
}
