#include "fft/menu.h"

/* Row condition: 1 when the ability that menu_ability_is_non_action_learned last cached is an action
 * ability (ABILITY_TYPE_DEFAULT). */
s32 menu_ability_is_action(s32 unused_row) {
    return g_menu_ability_type == ABILITY_TYPE_DEFAULT;
}
