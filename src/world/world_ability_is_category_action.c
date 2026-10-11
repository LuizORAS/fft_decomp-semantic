#include "fft/world.h"
#include "psx/types.h"

/* Report whether the ability screen's category tab is Action (g_world_ability_category 0); menu
 * script callback 13 of the view and learn lists. Its siblings test Reaction (1), Support (2) and
 * Movement (3). */
s32 world_ability_is_category_action(s32 unused_row) {
    return g_world_ability_category == MENU_ABILITY_CATEGORY_ACTION;
}
