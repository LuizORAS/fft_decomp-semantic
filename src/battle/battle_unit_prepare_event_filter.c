#include "fft/battle_unit.h"
#include "fft/script.h"
#include "psx/types.h"

/* Prepares an event-script unit operand for the unit filter loop.
 *
 * A single unit id (1..0xff) is replaced in place by its unit id with *filter_mode 0;
 * the result is 0 when it resolves to no unit. Id 0 (every unit) and the group
 * selectors from EVENT_UNIT_GROUP_FIRST are left as they are, with *filter_mode 1 for
 * id 0 and id - 0xfe for a group, and return 1. */
s32 battle_unit_prepare_event_filter(u16* unit_id, s32* filter_mode) {
    u32 id;

    id = *unit_id;
    if (id != 0 && id < EVENT_UNIT_GROUP_FIRST) {
        *unit_id = script_resolve_runtime_id(id);
        *filter_mode = 0;
        if (*unit_id == EVENT_RUNTIME_ID_NONE) {
            return 0;
        }
    } else {
        *filter_mode = id != 0 ? id - 0xfe : 1;
    }
    return 1;
}
