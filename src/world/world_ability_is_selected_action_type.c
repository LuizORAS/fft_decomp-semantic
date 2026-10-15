#include "fft/world.h"
#include "psx/types.h"

/* Report whether the list entry that world_ability_is_non_action_learned cached last is a default
 * ability (ids below 0x170, ABILITY_TYPE_DEFAULT); menu script callback 15 of the view and learn
 * lists. */
s32 world_ability_is_selected_action_type(s32 unused_row) {
    return g_world_ability_selected_type == 0;
}
