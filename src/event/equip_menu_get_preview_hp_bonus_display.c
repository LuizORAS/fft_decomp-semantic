#include "fft/event_equip.h"
#include "psx/types.h"

s32 equip_menu_get_preview_hp_bonus_display(s32 unused_row) {
    s32 value = g_equip_selected_unit_stat_summary.hp_bonus;

    if (value == 0) {
        return MENU_NUMBER_VALUE_DASHES;
    }
    return value;
}
