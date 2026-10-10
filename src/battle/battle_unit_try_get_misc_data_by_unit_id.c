#include "fft/battle.h"
#include "psx/types.h"

/* Prepares an event-script unit operand for the unit filter loop.
 *
 * A single unit id (1..0xff) is replaced in place by its misc id with *state 0;
 * the result is 0 when it resolves to no unit. Id 0 (every unit) and the group
 * selectors from EVENT_UNIT_GROUP_FIRST are left as they are, with *state 1 for
 * id 0 and id - 0xfe for a group, and return 1. */
s32 battle_unit_try_get_misc_data_by_unit_id(u16* unit_id, s32* state) {
    u32 id;

    id = *unit_id;
    if (id != 0 && id < EVENT_UNIT_GROUP_FIRST) {
        *unit_id = battle_script_get_misc_id(id);
        *state = 0;
        if (*unit_id == EVENT_MISC_ID_NONE) {
            return 0;
        }
    } else {
        *state = id != 0 ? id - 0xfe : 1;
    }
    return 1;
}
