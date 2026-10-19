#include "fft/menu.h"

/* Row condition: 1 while the ability list shows the action tab (MENU_ABILITY_CATEGORY_ACTION). */
s32 menu_ability_is_action_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_ACTION;
}
