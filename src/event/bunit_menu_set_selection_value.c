#include "fft/event_bunit.h"
#include "fft/menu.h"
#include "psx/types.h"

/* Set selector `index` to choice `value` and return it. */
s32 bunit_menu_set_selection_value(u8 index, s16 value) {
    return g_menu_selection_values[index] = value;
}
