#include "fft/menu.h"

/* Reaction abilities occupy category one. */
s32 menu_ability_is_reaction_tab(s32 unused_row) {
    return g_menu_ability_category == MENU_ABILITY_CATEGORY_REACTION;
}
