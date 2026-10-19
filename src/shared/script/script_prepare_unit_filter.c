#include "fft/script.h"

/* Resolve an event's unit id for script_filter_unit_id_by_mode. 1-0xff names one ENTD unit:
 * unit_id becomes its runtime id with filter_mode 0, and the result is 0 when it is not on the field. 0
 * selects every unit (filter_mode 1) and 0x100-0x103 a team (filter_mode 2-5, id - 0xfe: blue, blue without
 * the MAIN_STATUS_CHECK_SET_SCRIPT_FILTER_EXCLUDED statuses, the others, the others likewise). Returns 1
 * then. BATTLE and WORLD compile this text. */
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
