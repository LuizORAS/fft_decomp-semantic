#include "fft/battle.h"
#include "psx/types.h"

/* Add `amount` to a unit's event offset. The copy every caller uses;
 * battle_unit_add_event_offset_by_misc_id_unused is the same routine compiled twice. */
s32 battle_unit_add_event_offset_by_misc_id(u32 misc_id, const SVECTOR* amount) {
    battle_unit_misc_data_t* unit;

    unit = battle_unit_get_misc_data_by_misc_id(misc_id & 0xffff);
    if (unit != 0) {
        unit->event_offset.vx = unit->event_offset.vx + amount->vx;
        unit->event_offset.vy = unit->event_offset.vy + amount->vy;
        unit->event_offset.vz = unit->event_offset.vz + amount->vz;
    } else {
        main_system_handle_pointer_exception(12);
        return 0;
    }
    return 1;
}
