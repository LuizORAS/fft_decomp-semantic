#include "fft/world.h"
#include "psx/types.h"

s32 world_unit_try_get_misc_data_by_unit_id(u16* unit_id, s32* misc_index) {
    u16 id;

    id = *unit_id;
    if (id != 0 && id < EVENT_UNIT_GROUP_FIRST) {
        *unit_id = world_script_get_misc_id(id);
        *misc_index = 0;
        if (*unit_id == 2000) {
            return 0;
        }
    } else {
        *misc_index = (id != 0) ? id - 0xfe : 1;
    }
    return 1;
}
