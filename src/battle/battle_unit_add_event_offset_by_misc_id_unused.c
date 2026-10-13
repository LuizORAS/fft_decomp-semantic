#include "fft/battle_unit.h"
#include "fft/main.h"
#include "psx/types.h"

/* The same routine as battle_unit_add_event_offset_by_misc_id, compiled twice. Nothing calls this copy. */
s32 battle_unit_add_event_offset_by_misc_id_unused(u32 misc_id, const SVECTOR* amount) {
    battle_unit_t* unit;

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
