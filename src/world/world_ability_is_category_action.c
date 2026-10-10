#include "fft/world.h"
#include "psx/types.h"

/* The action-ability tab is category zero. */
s32 world_ability_is_category_action(s32 unused_row) {
    return g_world_ability_category == MENU_ABILITY_CATEGORY_ACTION;
}
