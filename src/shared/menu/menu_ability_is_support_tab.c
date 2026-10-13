#include "fft/menu.h"

/* Support abilities occupy category two. */
s32 menu_ability_is_support_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_SUPPORT;
}
