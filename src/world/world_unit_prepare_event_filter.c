#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Decode an event instruction's unit operand. A single unit id is replaced
 * by its unit id with *filter_mode 0; the result is 0 when that unit is not
 * displayed. Id 0 and the group ids leave the id alone and set the group
 * mode in *filter_mode (1 for id 0, id - 0xfe for a group) for
 * world_script_filter_unit_id_by_mode. */
s32 world_unit_prepare_event_filter(u16* unit_id, s32* filter_mode) {
    u16 id;

    id = *unit_id;
    if (id != 0 && id < EVENT_UNIT_GROUP_FIRST) {
        *unit_id = script_resolve_runtime_id(id);
        *filter_mode = 0;
        if (*unit_id == EVENT_RUNTIME_ID_NONE) {
            return 0;
        }
    } else {
        *filter_mode = (id != 0) ? id - 0xfe : 1;
    }
    return 1;
}
