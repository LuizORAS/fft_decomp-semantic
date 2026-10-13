#include "fft/menu.h"

/* Movement abilities occupy category three. */
s32 menu_ability_is_movement_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_MOVEMENT;
}
