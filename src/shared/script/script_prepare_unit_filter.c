#include "fft/script.h"

/* Resolve a single selector in place; groups keep their ID and set the filter mode. */
s32 script_prepare_unit_filter(u16* unit_id, s32* filter_mode) {
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
