#include "fft/menu.h"

/* The action-ability tab is category zero. */
s32 menu_ability_is_action_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_ACTION;
}
