#include "fft/battle.h"
#include "psx/types.h"

/* Resolve an event's unit id for battle_script_filter_unit_id_by_mode. 1-0xff names one ENTD unit:
 * unit_id becomes its misc id with state 0, and the result is 0 when it is not on the field. 0
 * selects every unit (state 1) and 0x100-0x103 a team (states 2-5: blue, blue without the
 * MAIN_STATUS_CHECK_SET_EVENT_EXCLUDED statuses, the others, the others likewise). Returns 1 then. */
s32 battle_unit_try_get_misc_data_by_unit_id(u16* unit_id, s32* state) {
    u32 id;

    id = *unit_id;
    if (id != 0 && id < 0x100) {
        *unit_id = battle_get_misc_id(id);
        *state = 0;
        if (*unit_id == EVENT_MISC_ID_NONE) {
            return 0;
        }
    } else {
        *state = id != 0 ? id - 0xfe : 1;
    }
    return 1;
}
