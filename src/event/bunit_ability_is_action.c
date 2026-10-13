#include "fft/event_bunit.h"
#include "psx/types.h"

/* Row condition: 1 when the ability that bunit_ability_is_non_action_learned last cached is an action
 * ability (ABILITY_TYPE_DEFAULT). */
s32 bunit_ability_is_action(s32 unused_row) {
    return g_bunit_ability_type == ABILITY_TYPE_DEFAULT;
}
