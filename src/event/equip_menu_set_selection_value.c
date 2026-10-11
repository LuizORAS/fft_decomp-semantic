#include "fft/event_equip.h"
#include "psx/types.h"

s32 equip_menu_set_selection_value(u8 index, s16 value) {
    g_equip_menu_selection_values[index] = value;
    return value;
}
