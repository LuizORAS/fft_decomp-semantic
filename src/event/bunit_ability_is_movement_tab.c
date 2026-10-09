#include "fft/event_bunit.h"
#include "psx/types.h"

/* Movement abilities occupy category three. */
s32 bunit_ability_is_movement_tab(s32 unused_row) {
    return g_bunit_ability_category == MENU_ABILITY_CATEGORY_MOVEMENT;
}
