#include "fft/menu.h"

/* Action abilities occupy type zero in the ability-pointer classification. */
s32 menu_ability_is_action(s32 unused_row) {
    return g_menu_ability_type == ABILITY_TYPE_DEFAULT;
}
