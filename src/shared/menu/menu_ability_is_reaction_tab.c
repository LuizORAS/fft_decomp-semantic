#include "fft/menu.h"

/* Row condition: 1 while the ability list shows the reaction tab (MENU_ABILITY_CATEGORY_REACTION). */
s32 menu_ability_is_reaction_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_REACTION;
}
