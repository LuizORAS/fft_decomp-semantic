#include "fft/event_bunit.h"
#include "psx/types.h"

/* Row condition: 1 while the ability list shows the support tab (MENU_ABILITY_CATEGORY_SUPPORT). */
s32 bunit_ability_is_support_tab(s32 unused_row) {
    return g_bunit_ability_category == MENU_ABILITY_CATEGORY_SUPPORT;
}
