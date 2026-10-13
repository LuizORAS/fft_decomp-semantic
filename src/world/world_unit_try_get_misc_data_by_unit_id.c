#include "fft/script.h"
#include "fft/world.h"
#include "psx/types.h"

/* Decode an event instruction's unit operand. A single unit id is replaced
 * by its misc id with *misc_index 0; the result is 0 when that unit is not
 * displayed. Id 0 and the group ids leave the id alone and set the group
 * mode in *misc_index (1 for id 0, id - 0xfe for a group) for
 * world_script_filter_unit_id_by_mode. */
s32 world_unit_try_get_misc_data_by_unit_id(u16* unit_id, s32* misc_index) {
    u16 id;

    id = *unit_id;
    if (id != 0 && id < EVENT_UNIT_GROUP_FIRST) {
        *unit_id = script_get_misc_id(id);
        *misc_index = 0;
        if (*unit_id == EVENT_RUNTIME_ID_NONE) {
            return 0;
        }
    } else {
        *misc_index = (id != 0) ? id - 0xfe : 1;
    }
    return 1;
}
