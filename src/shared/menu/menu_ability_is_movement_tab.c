#include "fft/menu.h"

/* Row condition: 1 while the ability list shows the movement tab (MENU_ABILITY_CATEGORY_MOVEMENT). */
s32 menu_ability_is_movement_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_MOVEMENT;
}
