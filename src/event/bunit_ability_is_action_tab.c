#include "fft/event_bunit.h"
#include "psx/types.h"

/* The action-ability tab is category zero. */
s32 bunit_ability_is_action_tab(s32 unused_row) {
    return g_bunit_ability_category == MENU_ABILITY_CATEGORY_ACTION;
}
